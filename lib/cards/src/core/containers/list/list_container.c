/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_container.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:33:42 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:34:48 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/containers/list_container.h"

t_returncode	list_container_init(
	struct s_list_container *container
)
{
	if (!container)
		return (RETURN_ERROR);
	container->cards = cdll_create();
	if (!container->cards)
		return (RETURN_FATAL);
	container->card_count = 0;
	return (RETURN_OK);
}

void			list_container_free(
	struct s_list_container *container
)
{
	if (!container)
		return ;
	if (container->cards)
		cdll_destroy(container->cards, free);
	container->cards = NULL;
	container->card_count = 0;
}
