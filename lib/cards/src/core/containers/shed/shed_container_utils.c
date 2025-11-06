/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shed_container_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:38:12 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:44:14 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/shed_container.h"

t_returncode	shed_add_face_down(
	struct s_shed_container *shed,
	struct s_card_desc card,
	size_t index
)
{
	if (!shed || !shed->face_down)
		return (RETURN_ERROR);
	if (shed->face_down[index].is_filled)
		return (RETURN_ERROR);
	shed->face_down[index].card = card;
	shed->face_down[index].is_filled = 1;
	shed->face_down_count++;
	return (RETURN_OK);
}

t_returncode	shed_add_face_up(
	struct s_shed_container *shed,
	struct s_card_desc card,
	size_t index
)
{
	if (!shed || !shed->face_up)
		return (RETURN_ERROR);
	if (shed->face_up[index].is_filled)
		return (RETURN_ERROR);
	shed->face_up[index].card = card;
	shed->face_up[index].is_filled = 1;
	shed->face_up_count++;
	return (RETURN_OK);
}

t_returncode	shed_add_card(
	struct s_shed_container *shed,
	struct s_card_desc card,
	size_t *index,
	int *face_down
)
{
	if (!shed || !shed->face_down || !shed->face_up)
		return (RETURN_ERROR);
	// Try to add to face down first
	for (size_t i = 0; i < shed->half_capacity; i++)
	{
		if (!shed->face_down[i].is_filled)
		{
			shed->face_down[i].card = card;
			shed->face_down[i].is_filled = 1;
			shed->face_down_count++;
			if (index)
				*index = i;
			if (face_down)
				*face_down = 1;
			return (RETURN_OK);
		}
	}
	// Try to add to face up
	for (size_t i = 0; i < shed->half_capacity; i++)
	{
		if (!shed->face_up[i].is_filled)
		{
			shed->face_up[i].card = card;
			shed->face_up[i].is_filled = 1;
			shed->face_up_count++;
			if (index)
				*index = i;
			if (face_down)
				*face_down = 0;
			return (RETURN_OK);
		}
	}
	return (RETURN_ERROR);
}

t_returncode	shed_remove_face_down(
	struct s_shed_container *shed,
	size_t index,
	struct s_card_desc *removed_card
)
{
	if (!shed || !shed->face_down)
		return (RETURN_ERROR);
	if (!shed->face_down[index].is_filled)
		return (RETURN_ERROR);
	if (removed_card)
		*removed_card = shed->face_down[index].card;
	shed->face_down[index].is_filled = 0;
	shed->face_down_count--;
	return (RETURN_OK);
}

t_returncode	shed_remove_face_up(
	struct s_shed_container *shed,
	size_t index,
	struct s_card_desc *removed_card
)
{
	if (!shed || !shed->face_up)
		return (RETURN_ERROR);
	if (!shed->face_up[index].is_filled)
		return (RETURN_ERROR);
	if (removed_card)
		*removed_card = shed->face_up[index].card;
	shed->face_up[index].is_filled = 0;
	shed->face_up_count--;
	return (RETURN_OK);
}

t_returncode	shed_remove_card(
	struct s_shed_container *shed,
	size_t index,
	int face_down,
	struct s_card_desc *removed_card
)
{
	if (face_down)
		return (shed_remove_face_down(shed, index, removed_card));
	else
		return (shed_remove_face_up(shed, index, removed_card));
}

t_returncode	shed_clear(
	struct s_shed_container *shed
)
{
	if (!shed)
		return (RETURN_ERROR);
	for (size_t i = 0; i < shed->half_capacity; i++)
	{
		shed->face_down[i].is_filled = 0;
		shed->face_up[i].is_filled = 0;
	}
	shed->face_down_count = 0;
	shed->face_up_count = 0;
	return (RETURN_OK);
}

t_returncode	shed_get_card(
	struct s_shed_container *shed,
	size_t index,
	int face_down,
	struct s_card_desc *out_card
)
{
	if (!shed)
		return (RETURN_ERROR);
	if (face_down)
	{
		if (!shed->face_down || !shed->face_down[index].is_filled)
			return (RETURN_ERROR);
		if (out_card)
			*out_card = shed->face_down[index].card;
	}
	else
	{
		if (!shed->face_up || !shed->face_up[index].is_filled)
			return (RETURN_ERROR);
		if (out_card)
			*out_card = shed->face_up[index].card;
	}
	return (RETURN_OK);
}

t_returncode	shed_has_card(
	struct s_shed_container *shed,
	struct s_card_desc card,
	int *face_down
)
{
	if (!shed)
		return (0);
	for (size_t i = 0; i < shed->half_capacity; i++)
	{
		if (shed->face_down[i].is_filled)
		{
			if (shed->face_down[i].card.rank == card.rank
				&& shed->face_down[i].card.suit == card.suit)
			{
				if (face_down)
					*face_down = 1;
				return (1);
			}
		}
	}
	for (size_t i = 0; i < shed->half_capacity; i++)
	{
		if (shed->face_up[i].is_filled)
		{
			if (shed->face_up[i].card.rank == card.rank
				&& shed->face_up[i].card.suit == card.suit)
			{
				if (face_down)
					*face_down = 0;
				return (1);
			}
		}
	}
	return (0);
}
