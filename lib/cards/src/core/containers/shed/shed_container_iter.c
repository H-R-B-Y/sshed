/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shed_container_iter.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:51:05 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:51:45 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/shed_container.h"

t_returncode	shed_container_foreach(
	struct s_shed_container *container,
	int (*callback)(struct s_card_desc card, size_t index, int face_down, void *data),
	void *user_data
)
{
	size_t	i;
	int		result;

	if (!container || !container->face_down || !container->face_up || !callback)
		return (RETURN_ERROR);
	// Iterate face down cards
	for (i = 0; i < container->half_capacity; i++)
	{
		if (container->face_down[i].is_filled)
		{
			result = callback(
				container->face_down[i].card,
				i,
				1,
				user_data
			);
			if (result != 0)
				return (RETURN_ERROR);
		}
	}
	// Iterate face up cards
	for (i = 0; i < container->half_capacity; i++)
	{
		if (container->face_up[i].is_filled)
		{
			result = callback(
				container->face_up[i].card,
				i,
				0,
				user_data
			);
			if (result != 0)
				return (RETURN_ERROR);
		}
	}
	return (RETURN_OK);
}
