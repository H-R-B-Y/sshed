/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container_get.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:37:03 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 17:07:25 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/container.h"

t_returncode	container_get_all_cards(
	struct s_container *container,
	struct s_card_desc **out_cards,
	size_t *out_count
)
{
	struct s_container	*cont;

	if (!container || !out_cards || !out_count)
		return (RETURN_ERROR);
	switch (container->type)
	{
		case (CONTAINER_TYPE_ARRAY):
		{
			struct s_array_container	*array_cont;

			array_cont = &container->data.array_container;
			*out_count = array_cont->card_count;
			*out_cards = ft_memdup(
				array_cont->cards,
				array_cont->card_count * sizeof(struct s_card_desc)
			);
			if (!*out_cards)
				return (RETURN_FATAL);
			return (RETURN_OK);
		}
		case (CONTAINER_TYPE_LIST):
		{
			struct s_list_container	*list_cont;
			size_t					i;
			struct s_cdll_node		*current;
			struct s_card_desc		*cards_array;
			list_cont = &container->data.list_container;
			*out_count = list_cont->card_count;
			cards_array = ft_calloc(
				list_cont->card_count,
				sizeof(struct s_card_desc)
			);
			if (!cards_array)
				return (RETURN_FATAL);
			current = list_cont->cards->head;
			i = 0;
			while (current)
			{
				cards_array[i] = *(struct s_card_desc *)current->data;
				current = current->next;
				i++;
			}
			*out_cards = cards_array;
			return (RETURN_OK);
		}
		case (CONTAINER_TYPE_SHED):
		{
			t_u8	idx;
			size_t	total_count;
			struct s_card_desc	*cards_array;
			struct s_shed_container	*shed_cont;
			shed_cont = &container->data.shed_container;
			total_count = shed_cont->face_down_count + shed_cont->face_up_count;
			*out_count = total_count;
			cards_array = ft_calloc(
				total_count,
				sizeof(struct s_card_desc)
			);
			if (!cards_array)
				return (RETURN_FATAL);
			idx = 0;
			for (size_t i = 0; i < shed_cont->half_capacity; i++)
			{
				if (shed_cont->face_down[i].is_filled)
				{
					cards_array[idx] = shed_cont->face_down[i].card;
					idx++;
				}
			}
			for (size_t i = 0; i < shed_cont->half_capacity; i++)
			{
				if (shed_cont->face_up[i].is_filled)
				{
					cards_array[idx] = shed_cont->face_up[i].card;
					idx++;
				}
			}
			*out_cards = cards_array;
			return (RETURN_OK);
		}
		default:
			return (RETURN_ERROR);
	}
}

t_returncode	container_get_count(
	struct s_container *container,
	size_t *out_count
)
{
	if (!container || !out_count)
		return (RETURN_ERROR);
	switch (container->type)
	{
		case (CONTAINER_TYPE_ARRAY):
			*out_count = container->data.array_container.card_count;
			return (RETURN_OK);
		case (CONTAINER_TYPE_LIST):
			*out_count = container->data.list_container.card_count;
			return (RETURN_OK);
		case (CONTAINER_TYPE_SHED):
			*out_count = container->data.shed_container.face_down_count
				+ container->data.shed_container.face_up_count;
			return (RETURN_OK);
		case (CONTAINER_TYPE_COUNT_ONLY):
			*out_count = container->data.count_container.card_count;
			return (RETURN_OK);
		default:
			return (RETURN_ERROR);
	}
}

// There has to be a better way to do this
// TODO: rework this function it sucks balls
t_returncode	container_get_cards(
	struct s_container *container,
	t_card_filter_func *filter,
	void *user_data,
	struct s_card_desc **out_cards,
	size_t *out_count
)
{
	struct s_card_desc	*all_cards;
	size_t				total_count;
	size_t				matched_count;
	size_t				i;

	if (!container || !out_cards || !out_count)
		return (RETURN_ERROR);
	if (container_get_all_cards(container, &all_cards, &total_count) != RETURN_OK)
		return (RETURN_FATAL);
	matched_count = 0;
	// First pass to count matches
	for (i = 0; i < total_count; i++)
	{
		if (!filter || filter(all_cards[i], user_data))
			matched_count++;
	}
	*out_count = matched_count;
	*out_cards = ft_calloc(matched_count, sizeof(struct s_card_desc));
	if (!*out_cards)
	{
		free(all_cards);
		return (RETURN_FATAL);
	}
	// Second pass to collect matches
	matched_count = 0;
	for (i = 0; i < total_count; i++)
	{
		if (!filter || filter(all_cards[i], user_data))
		{
			(*out_cards)[matched_count] = all_cards[i];
			matched_count++;
		}
	}
	free(all_cards);
	return (RETURN_OK);
}
