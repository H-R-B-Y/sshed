/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_plane.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 10:51:03 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:07:16 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "notcurses/card_plane.h"

extern t_returncode (*g_card_visual_loder)(
	struct notcurses *nc,
	struct s_card_visual *visual,
	struct s_card_desc card,
	const char *visual_directory
) = NULL;

t_returncode card_visual_load(
	struct notcurses *nc,
	struct s_card_visual *visual,
	struct s_card_desc card,
	const char *visual_directory
)
{
	if (!visual || !g_card_visual_loder)
		return (RETURN_ERROR);
	return (g_card_visual_loder(nc, visual, card, visual_directory));
}

t_returncode card_set_load_visuals(
	struct notcurses *nc,
	struct s_card_set *set,
	const char *visual_directory
)
{
	int	dimensions_set;

	dimensions_set = 0;
	if (!set)
		return (RETURN_ERROR);
	// Iterate over all cards in the set and load their visuals
	for (size_t i = 0; i < set->card_count; i++)
	{
		// this array has all the card descriptions
		// we just need to add the visuals to the hashmap
		struct s_card_desc card = set->cards[i];
		struct s_card_visual *visual = ft_calloc(1, sizeof(struct s_card_visual));
		if (!visual)
			return (RETURN_FATAL);
		t_returncode rc = card_visual_load(nc, visual, card, visual_directory);
		if (rc != RETURN_OK)
		{
			free(visual);
			return (rc);
		}
		// Add to hashmap
		hm_add_value(
			&set->card_map,
			&card,
			visual
		);
		if (!dimensions_set)
		{
			struct ncvisual_options vopts = (struct ncvisual_options){
				.flags = NCVISUAL_OPTION_CHILDPLANE | NCVISUAL_OPTION_NODEGRADE,
				.n = NULL,
				.blitter = NCBLIT_PIXEL
			};
			struct ncplane *temp_plane;
			for (enum e_card_orientation o = 0;
				o < CARD_ORIENTATION_COUNT;
				o++
			)
			{
				temp_plane = ncvisual_blit(nc, visual->visual[o], &vopts);
				unsigned int w, h;
				ncplane_dim_yx(temp_plane, &h, &w);
				g_card_plane_width[o] = w;
				g_card_plane_height[o] = h;
				ncplane_destroy(temp_plane);
			}
			dimensions_set = 1;
		}
	}
	return (RETURN_OK);
}

struct s_card_visual* card_set_get_visual(
	struct s_card_set *set,
	struct s_card_desc card
)
{
	if (!set)
		return (NULL);
	return (hm_get_value(
		&set->card_map,
		&card
	));
}

void	card_visual_free(
	struct s_card_visual *visual
)
{
	if (!visual)
		return ;
	for (size_t i = 0; i < CARD_ORIENTATION_COUNT; i++)
	{
		if (visual->visual[i])
		{
			ncvisual_destroy(visual->visual[i]);
			visual->visual[i] = NULL;
		}
	}
	free(visual);
}
