/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_container.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:00:05 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 16:49:00 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_CONTAINER_H
# define LIST_CONTAINER_H

# include "core/card.h"

/**
 * @brief List container using a doubly linked list
 */
struct s_list_container
{
	/// @brief Doubly linked list of cards
	t_cdll	*cards;
	/// @brief Current number of cards in the list
	size_t	card_count;
};

/**
 * @brief Initialize a list container
 * 
 * @param container Container to initialize
 * @return t_returncode 
 */
t_returncode	list_container_init(
	struct s_list_container *container
);

/**
 * @brief Free a list container
 * 
 * @param container 
 */
void			list_container_free(
	struct s_list_container *container
);

/**
 * @brief Add a card to the list container
 * 
 * @param container 
 * @param card 
 * @return t_returncode 
 */
t_returncode	list_container_add_card(
	struct s_list_container *container,
	struct s_card_desc card
);

/**
 * @brief 	Remove a card from the list container
 * 
 * @param container 
 * @param card 
 * @return t_returncode 
 */
t_returncode	list_container_remove_card(
	struct s_list_container *container,
	struct s_card_desc card
);

/**
 * @brief Check if the list container has a specific card
 * 
 * @param container Container to check
 * @param card Card to check for
 * @return t_returncode 
 */
int	list_container_has_card(
	struct s_list_container *container,
	struct s_card_desc card
);

/**
 * @brief Iterate over all cards in the list container and call the callback for each
 * 
 * @param container Container to iterate over
 * @param callback Callback function to call for each card
 * @param user_data User data to pass to the callback
 * @return t_returncode 
 */
t_returncode	list_container_foreach(
	struct s_list_container *container,
	int (*callback)(struct s_card_desc card, size_t index, void *data),
	void *user_data
);

#endif
