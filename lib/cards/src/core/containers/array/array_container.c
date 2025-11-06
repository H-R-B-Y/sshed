/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_container.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:30:58 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:31:13 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/array_container.h"

t_returncode	array_container_init(
	struct s_array_container *container,
	size_t initial_capacity
)
{
	if (!container)
		return (RETURN_ERROR);
	container->cards = ft_calloc(initial_capacity, sizeof(struct s_card_desc));
	if (!container->cards)
		return (RETURN_FATAL);
	container->card_count = 0;
	container->capacity = initial_capacity;
	return (RETURN_OK);
}

void			array_container_free(
	struct s_array_container *container
)
{
	if (!container)
		return ;
	if (container->cards)
		free(container->cards);
	container->cards = NULL;
	container->card_count = 0;
	container->capacity = 0;
}
