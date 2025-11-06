/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_plane_util.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:13:35 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:29:55 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "notcurses/card_plane.h"

t_returncode card_plane_render(
	struct s_card_plane *card_plane
)
{
	struct notcurses		*nc;
	struct ncvisual			*visual;
	struct s_card_visual	*card_visual;

	if (!card_plane)
		return (RETURN_ERROR);
	nc = ncplane_notcurses(card_plane->parent);
	if (card_plane->plane)
	{
		ncplane_erase(card_plane->plane);
		ncplane_destroy(card_plane->plane);
		card_plane->plane = NULL;
	}
	if (card_plane->face_down)
		card_visual = (struct s_card_visual *)card_plane->card_set->set_user_data;
	else
		card_visual = card_set_get_visual(card_plane->card_set, card_plane->card);
	if (!card_visual)
		return (RETURN_ERROR);
	visual = card_visual->visual[card_plane->orientation];
	card_plane = ncvisual_blit(nc, visual, &(struct ncvisual_options){.n = card_plane->parent,.y = 0,.x = 0,.blitter = NCBLIT_PIXEL,.scaling = NCSCALE_NONE,.flags = NCVISUAL_OPTION_CHILDPLANE | NCVISUAL_OPTION_NODEGRADE});
	if (!card_plane)
		return (RETURN_FATAL);
	card_plane->visible = 1;
	return (RETURN_OK);
}

t_returncode card_plane_erase(
	struct s_card_plane *card_plane
)
{
	if (!card_plane || !card_plane->plane)
		return (RETURN_ERROR);
	ncplane_erase(card_plane->plane);
	ncplane_destroy(card_plane->plane);
	card_plane->plane = NULL;
	card_plane->visible = 0;
	return (RETURN_OK);
}

t_returncode card_plane_move(
	struct s_card_plane *card_plane,
	int y,
	int x
)
{
	ncplane_move_yx(card_plane->plane, y, x);
	return (RETURN_OK);
}
