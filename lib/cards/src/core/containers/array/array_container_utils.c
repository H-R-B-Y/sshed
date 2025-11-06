/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_container_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:31:26 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:37:12 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/array_container.h"

t_returncode	array_container_add_card(
	struct s_array_container *container,
	struct s_card_desc card
)
{
	if (!container || !container->cards)
		return (RETURN_ERROR);
	if (container->card_count == container->capacity)
		return (RETURN_ERROR);
	container->cards[container->card_count] = card;
	container->card_count++;
	return (RETURN_OK);
}

t_returncode	array_container_remove_card(
	struct s_array_container *container,
	struct s_card_desc card
)
{
	size_t	i;

	if (!container || !container->cards)
		return (RETURN_ERROR);
	for (i = 0; i < container->card_count; i++)
	{
		if (container->cards[i].rank == card.rank
			&& container->cards[i].suit == card.suit)
		{
			// Shift remaining cards down
			for (; i < container->card_count - 1; i++)
			{
				container->cards[i] = container->cards[i + 1];
			}
			container->card_count--;
			return (RETURN_OK);
		}
	}
	return (RETURN_ERROR);
}


int				array_container_has_card(
	struct s_array_container *container,
	struct s_card_desc card
)
{
	size_t	i;

	if (!container || !container->cards)
		return (0);
	for (i = 0; i < container->card_count; i++)
	{
		if (container->cards[i].rank == card.rank
			&& container->cards[i].suit == card.suit)
		{
			return (1);
		}
	}
	return (0);
}

t_returncode	array_container_clear(
	struct s_array_container *container
)
{
	if (!container)
		return (RETURN_ERROR);
	container->card_count = 0;
	return (RETURN_OK);
}
