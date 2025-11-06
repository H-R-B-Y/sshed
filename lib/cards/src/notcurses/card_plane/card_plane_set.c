/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_plane_set.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:10:55 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:13:12 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "notcurses/card_plane.h"

t_returncode card_plane_set_card(
	struct s_card_plane *card_plane,
	struct s_card_set *card_set,
	struct s_card_desc card
)
{
	if (!card_plane || !card_set)
		return (RETURN_ERROR);
	card_plane->card = card;
	if (card_plane->visible)
	{
		card_plane_erase(card_plane);
		card_plane_render(card_plane);
	}
	return (RETURN_OK);
}

t_returncode card_plane_set_orientation(
	struct s_card_plane *card_plane,
	enum e_card_orientation orientation
)
{
	if (!card_plane)
		return (RETURN_ERROR);
	card_plane->orientation = orientation;
	if (card_plane->visible)
	{
		card_plane_erase(card_plane);
		card_plane_render(card_plane);
	}
	return (RETURN_OK);
}

t_returncode card_plane_set_face_down(
	struct s_card_plane *card_plane,
	t_u8 face_down
)
{
	if (!card_plane)
		return (RETURN_ERROR);
	card_plane->face_down = face_down;
	if (card_plane->visible)
	{
		card_plane_erase(card_plane);
		card_plane_render(card_plane);
	}
	return (RETURN_OK);
}

t_returncode card_plane_set_visible(
	struct s_card_plane *card_plane,
	t_u8 visible
)
{
	if (!card_plane)
		return (RETURN_ERROR);
	card_plane->visible = visible;
	if (visible)
		return (card_plane_render(card_plane));
	else
		return (card_plane_erase(card_plane));
}
