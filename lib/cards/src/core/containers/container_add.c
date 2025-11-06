/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container_add.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:55:19 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 17:08:17 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/container.h"

t_returncode	container_add_card(
	struct s_container *container,
	struct s_card_desc card
)
{
	switch (container->type)
	{
		case CONTAINER_TYPE_ARRAY:
			return (array_container_add_card(&container->data.array_container, card));
		case CONTAINER_TYPE_LIST:
			return (list_container_add_card(&container->data.list_container, card));
		case CONTAINER_TYPE_SHED:
		{
			size_t	index;
			int		face_down;
			return (shed_add_card(&container->data.shed_container, card, &index, &face_down));
		}
		case CONTAINER_TYPE_COUNT_ONLY:
			return (count_container_add(&container->data.count_container, 1));
		default:
			return (RETURN_ERROR);
	}
}

t_returncode	container_remove_card(
	struct s_container *container,
	struct s_card_desc card
)
{
	switch (container->type)
	{
		case CONTAINER_TYPE_ARRAY:
			return (array_container_remove_card(&container->data.array_container, card));
		case CONTAINER_TYPE_LIST:
			return (list_container_remove_card(&container->data.list_container, card));
		case CONTAINER_TYPE_COUNT_ONLY:
			return (count_container_remove(&container->data.count_container, 1));
		case CONTAINER_TYPE_SHED:
			// Removal from shed not implemented
			return (RETURN_ERROR);
		default:
			return (RETURN_ERROR);
	}
}

int	container_has_card(
	struct s_container *container,
	struct s_card_desc card
)
{
	switch (container->type)
	{
		case CONTAINER_TYPE_ARRAY:
			return (array_container_has_card(&container->data.array_container, card));
		case CONTAINER_TYPE_LIST:
			return (list_container_has_card(&container->data.list_container, card));
		case CONTAINER_TYPE_SHED:
			return (shed_has_card(&container->data.shed_container, card, NULL));
		default:
			return (0);
	}
}

t_returncode	container_clear(
	struct s_container *container
)
{
	switch (container->type)
	{
		case CONTAINER_TYPE_ARRAY:
			return (array_container_clear(&container->data.array_container));
		case CONTAINER_TYPE_LIST:
			return (list_container_clear(&container->data.list_container));
		case CONTAINER_TYPE_SHED:
			return (shed_clear(&container->data.shed_container));
		case CONTAINER_TYPE_COUNT_ONLY:
			return (count_container_clear(&container->data.count_container));
		default:
			return (RETURN_ERROR);
	}
}
