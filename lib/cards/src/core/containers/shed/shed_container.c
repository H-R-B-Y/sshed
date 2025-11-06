/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shed_container.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:37:28 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:38:00 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/shed_container.h"

t_returncode	shed_container_init(
	struct s_shed_container *shed,
	t_u8 half_capacity
)
{
	if (!shed)
		return (RETURN_ERROR);
	shed->half_capacity = half_capacity;
	shed->face_down = ft_calloc(half_capacity, sizeof(struct s_card_slot));
	if (!shed->face_down)
		return (RETURN_FATAL);
	shed->face_down_count = 0;
	shed->face_up = ft_calloc(half_capacity, sizeof(struct s_card_slot));
	if (!shed->face_up)
	{
		free(shed->face_down);
		return (RETURN_FATAL);
	}
	shed->face_up_count = 0;
	return (RETURN_OK);
}

void			shed_container_free(
	struct s_shed_container *shed
)
{
	if (!shed)
		return ;
	if (shed->face_down)
		free(shed->face_down);
	shed->face_down = NULL;
	shed->face_down_count = 0;
	if (shed->face_up)
		free(shed->face_up);
	shed->face_up = NULL;
	shed->face_up_count = 0;
}
