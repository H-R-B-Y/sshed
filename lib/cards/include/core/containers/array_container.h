/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_container.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:11:11 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 14:54:47 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_CONTAINER_H
# define ARRAY_CONTAINER_H

# include "core/card.h"

/**
 * @brief Array container using a dynamic array
 */
struct s_array_container
{
	/// @brief Dynamic array of cards
	struct s_card_desc	*cards;
	/// @brief Current number of cards in the array
	size_t				card_count;
	/// @brief Current capacity of the array container
	size_t				capacity;
};

/**
 * @brief Initialize a array container
 * 
 * @param container Container to initialize
 * @param initial_capacity Initial capacity of the container
 * @return t_returncode 
 */
t_returncode	array_container_init(
	struct s_array_container *container,
	size_t initial_capacity
);

/**
 * @brief Free a array container
 * 
 * @param container Container to free
 */
void			array_container_free(
	struct s_array_container *container
);

/**
 * @brief Add a card to the array container
 * 
 * @param container Container to add the card to
 * @param card Card to add
 * @return t_returncode 
 */
t_returncode	array_container_add_card(
	struct s_array_container *container,
	struct s_card_desc card
);

/**
 * @brief Remove a card from the array container
 * 
 * @param container Container to remove the card from
 * @param card Card to remove
 * @return t_returncode 
 */
t_returncode	array_container_remove_card(
	struct s_array_container *container,
	struct s_card_desc card
);

/**
 * @brief Check if the array container contains a specific card
 * 
 * @param container Container to check
 * @param card Card to check for
 * @return int 
 */
int				array_container_has_card(
	struct s_array_container *container,
	struct s_card_desc card
);

/**
 * @brief Clear all cards from the array container
 * 
 * @param container Container to clear
 * @return t_returncode 
 */
t_returncode	array_container_clear(
	struct s_array_container *container
);

/**
 * @brief Iterate over all cards in the array container and call the callback for each
 * 
 * @param container Container to iterate over
 * @param callback Callback function to call for each card
 * @param user_data User data to pass to the callback
 * @return t_returncode 
 */
t_returncode	array_container_foreach(
	struct s_array_container *container,
	int (*callback)(struct s_card_desc card, size_t index, void *data),
	void *user_data
);

HEADER_STATIC_CONST
size_t	array_container_get_count(
	struct s_array_container *container
)
{
	return (container->card_count);
}

#endif
