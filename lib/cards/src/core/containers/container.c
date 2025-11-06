/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:44:57 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 17:07:25 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/container.h"

t_returncode	container_init(
	struct s_container *container,
	enum e_container_type type,
	size_t initial_capacity // only used for array and shed
)
{
	if (!container)
		return (RETURN_ERROR);
	container->type = type;
	switch (type)
	{
		case CONTAINER_TYPE_ARRAY:
			return (array_container_init(&container->data.array_container, initial_capacity));
		case CONTAINER_TYPE_LIST:
			return (list_container_init(&container->data.list_container));
		case CONTAINER_TYPE_SHED:
			return (shed_container_init(&container->data.shed_container, (t_u8)initial_capacity));
		case CONTAINER_TYPE_COUNT_ONLY:
			return (count_container_init(&container->data.count_container, initial_capacity));
		default:
			return (RETURN_ERROR);
	}
}

void	container_free(
	struct s_container *container
)
{
	if (!container)
		return ;
	switch (container->type)
	{
		case CONTAINER_TYPE_ARRAY:
			array_container_free(&container->data.array_container);
			break;
		case CONTAINER_TYPE_LIST:
			list_container_free(&container->data.list_container);
			break;
		case CONTAINER_TYPE_SHED:
			shed_container_free(&container->data.shed_container);
			break;
		case CONTAINER_TYPE_COUNT_ONLY:
			count_container_free(&container->data.count_container);
			break;
		default:
			break;
	}
}
