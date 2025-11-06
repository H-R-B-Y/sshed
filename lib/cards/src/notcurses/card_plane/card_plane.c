/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_plane.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:08:54 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:23:36 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "notcurses/card_plane.h"

t_returncode card_plane_create(
	struct s_card_plane **out_card_plane,
	struct ncplane *parent,
	struct s_card_set *card_set,
	struct s_card_desc card,
	enum e_card_orientation orientation,
	t_u8 face_down
)
{
	struct s_card_plane	*card_plane;

	if (!out_card_plane || !parent || !card_set)
		return (RETURN_ERROR);
	card_plane = ft_calloc(1, sizeof(struct s_card_plane));
	if (!card_plane)
		return (RETURN_FATAL);
	card_plane->parent = parent;
	card_plane->plane = NULL;
	card_plane->card_set = card_set;
	card_plane->card = card;
	card_plane->orientation = orientation;
	card_plane->face_down = face_down;
	card_plane->visible = 0;
	*out_card_plane = card_plane;
	return (RETURN_OK);
}

void	card_plane_destroy(
	struct s_card_plane *card_plane
)
{
	if (!card_plane)
		return ;
	if (card_plane->plane)
	{
		ncplane_destroy(card_plane->plane);
		card_plane->plane = NULL;
	}
	free(card_plane);
}
