/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_container.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:00:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 17:07:24 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/containers/count_container.h"

t_returncode	count_container_init(
	struct s_count_container *container,
	size_t initial_count
)
{
	if (!container)
		return (RETURN_ERROR);
	container->card_count = initial_count;
	return (RETURN_OK);
}

void	count_container_free(
	struct s_count_container *container
)
{
	if (!container)
		return ;
	container->card_count = 0;
}

t_returncode	count_container_add(
	struct s_count_container *container,
	size_t amount
)
{
	if (!container)
		return (RETURN_ERROR);
	container->card_count += amount;
	return (RETURN_OK);
}

t_returncode	count_container_remove(
	struct s_count_container *container,
	size_t amount
)
{
	if (!container)
		return (RETURN_ERROR);
	if (container->card_count < amount)
		return (RETURN_ERROR);
	container->card_count -= amount;
	return (RETURN_OK);
}

t_returncode	count_container_set(
	struct s_count_container *container,
	size_t count
)
{
	if (!container)
		return (RETURN_ERROR);
	container->card_count = count;
	return (RETURN_OK);
}

t_returncode	count_container_clear(
	struct s_count_container *container
)
{
	if (!container)
		return (RETURN_ERROR);
	container->card_count = 0;
	return (RETURN_OK);
}
