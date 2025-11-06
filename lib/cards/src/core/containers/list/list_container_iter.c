/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_container_iter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:50:33 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:50:50 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/list_container.h"

t_returncode	list_container_foreach(
	struct s_list_container *container,
	int (*callback)(struct s_card_desc card, size_t index, void *data),
	void *user_data
)
{
	struct s_cdll_node	*current;
	struct s_card_desc	*current_card;
	size_t				index;
	int					result;

	if (!container || !container->cards || !callback)
		return (RETURN_ERROR);
	current = container->cards->head;
	index = 0;
	while (current)
	{
		current_card = (struct s_card_desc *)current->data;
		result = callback(*current_card, index, user_data);
		if (result != 0)
			return (RETURN_ERROR);
		current = current->next;
		index++;
	}
	return (RETURN_OK);
}
