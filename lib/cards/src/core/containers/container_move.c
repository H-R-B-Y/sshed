/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container_move.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:45:40 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:54:56 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/container.h"

t_returncode array_container_move_cards(
	struct s_array_container *src,
	struct s_container *dst,
	t_card_filter_func *filter,
	void *user_data
)
{
	size_t			i;
	t_returncode	result;

	i = 0;
	while (i < src->card_count)
	{
		if (!filter || filter(src->cards[i], user_data))
		{
			result = container_add_card(dst, src->cards[i]);
			if (result != RETURN_OK)
				return (result);
			// Remove card from source
			array_container_remove_card(src, src->cards[i]);
		}
		i++;
	}
}

t_returncode list_container_move_cards(
	struct s_list_container *src,
	struct s_container *dst,
	t_card_filter_func *filter,
	void *user_data
)
{
	struct s_cdll_node	*current;
	struct s_cdll_node	*next;
	struct s_card_desc	*current_card;
	t_returncode		result;

	if (!src || !src->cards)
		return (RETURN_ERROR);
	current = src->cards->head;
	while (current)
	{
		next = current->next;
		current_card = (struct s_card_desc *)current->data;
		if (!filter || filter(*current_card, user_data))
		{
			result = container_add_card(dst, *current_card);
			if (result != RETURN_OK)
				return (result);
			// Remove card from source
			cdll_remove_node(src->cards, current, free);
			src->card_count--;
		}
		current = next;
	}
	return (RETURN_OK);
}

t_returncode shed_container_move_cards(
	struct s_shed_container *src,
	struct s_container *dst,
	t_card_filter_func *filter,
	void *user_data
)
{
	t_returncode	result;

	for (int i = 0;
		i < (int)src->half_capacity;
		i++
	)
	{
		if (src->face_down[i].is_filled)
		{
			if (!filter || filter(src->face_down[i].card, user_data))
			{
				result = container_add_card(dst, src->face_down[i].card);
				if (result != RETURN_OK)
					return (result);
				// Remove card from source
				src->face_down[i].is_filled = 0;
				src->face_down_count--;
			}
		}
	}
	for (int i = 0;
		i < (int)src->half_capacity;
		i++
	)
	{
		if (src->face_up[i].is_filled)
		{
			if (!filter || filter(src->face_up[i].card, user_data))
			{
				result = container_add_card(dst, src->face_up[i].card);
				if (result != RETURN_OK)
					return (result);
				// Remove card from source
				src->face_up[i].is_filled = 0;
				src->face_up_count--;
			}
		}
	}
	return (RETURN_OK);
}

t_returncode	container_move_cards(
	struct s_container *src,
	struct s_container *dst,
	t_card_filter_func *filter,
	void *user_data
)
{
	if (!src || !dst)
		return (RETURN_ERROR);
	switch (src->type)
	{
		case CONTAINER_TYPE_ARRAY:
			return (array_container_move_cards(
				&src->data.array_container,
				dst,
				filter,
				user_data
			));
		case CONTAINER_TYPE_LIST:
			return (list_container_move_cards(
				&src->data.list_container,
				dst,
				filter,
				user_data
			));
		case CONTAINER_TYPE_SHED:
			return (shed_container_move_cards(
				&src->data.shed_container,
				dst,
				filter,
				user_data
			));
		default:
			return (RETURN_ERROR);
	}
}

t_returncode	container_transfer_all(
	struct s_container *src,
	struct s_container *dst
)
{
	return (container_move_cards(src, dst, NULL, NULL));
}
