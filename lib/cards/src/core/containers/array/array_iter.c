/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_iter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:49:29 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:49:53 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/array_container.h"

t_returncode	array_container_foreach(
	struct s_array_container *container,
	int (*callback)(struct s_card_desc card, size_t index, void *data),
	void *user_data
)
{
	size_t	i;
	int		result;

	if (!container || !container->cards || !callback)
		return (RETURN_ERROR);
	for (i = 0; i < container->card_count; i++)
	{
		result = callback(container->cards[i], i, user_data);
		if (result != 0)
			return (RETURN_ERROR);
	}
	return (RETURN_OK);
}
