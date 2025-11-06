/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_container_uitls.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:34:51 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:36:53 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/list_container.h"

t_returncode	list_container_add_card(
	struct s_list_container *container,
	struct s_card_desc card
)
{
	if (!container || !container->cards)
		return (RETURN_ERROR);
	if (cdll_add_back(container->cards, ft_memdup(&card, sizeof(struct s_card_desc))) != RETURN_OK)
		return (RETURN_FATAL);
	container->card_count++;
	return (RETURN_OK);
}

t_returncode	list_container_remove_card(
	struct s_list_container *container,
	struct s_card_desc card
)
{
	struct s_cdll_node	*current;
	struct s_card_desc	*current_card;

	if (!container || !container->cards)
		return (RETURN_ERROR);
	current = container->cards->head;
	while (current)
	{
		current_card = (struct s_card_desc *)current->data;
		if (current_card->rank == card.rank && current_card->suit == card.suit)
		{
			cdll_remove_node(container->cards, current, free);
			container->card_count--;
			return (RETURN_OK);
		}
		current = current->next;
	}
	return (RETURN_ERROR);
}

int	list_container_has_card(
	struct s_list_container *container,
	struct s_card_desc card
)
{
	struct s_cdll_node	*current;
	struct s_card_desc	*current_card;

	if (!container || !container->cards)
		return (0);
	current = container->cards->head;
	while (current)
	{
		current_card = (struct s_card_desc *)current->data;
		if (current_card->rank == card.rank && current_card->suit == card.suit)
		{
			return (1);
		}
		current = current->next;
	}
	return (0);
}
