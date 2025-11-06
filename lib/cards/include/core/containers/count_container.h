/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_container.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:00:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 17:07:24 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COUNT_CONTAINER_H
# define COUNT_CONTAINER_H

# include "core/card.h"

/**
 * @brief Count container for tracking unknown cards (e.g., opponent hands)
 */
struct s_count_container
{
	/// @brief Number of cards in the container
	size_t	card_count;
};

/**
 * @brief Initialize a count container
 * 
 * @param container Container to initialize
 * @param initial_count Initial card count
 * @return t_returncode 
 */
t_returncode	count_container_init(
	struct s_count_container *container,
	size_t initial_count
);

/**
 * @brief Free a count container
 * 
 * @param container Container to free
 */
void			count_container_free(
	struct s_count_container *container
);

/**
 * @brief Increment the card count
 * 
 * @param container Container to modify
 * @param amount Amount to add
 * @return t_returncode 
 */
t_returncode	count_container_add(
	struct s_count_container *container,
	size_t amount
);

/**
 * @brief Decrement the card count
 * 
 * @param container Container to modify
 * @param amount Amount to remove
 * @return t_returncode 
 */
t_returncode	count_container_remove(
	struct s_count_container *container,
	size_t amount
);

/**
 * @brief Set the card count
 * 
 * @param container Container to modify
 * @param count New card count
 * @return t_returncode 
 */
t_returncode	count_container_set(
	struct s_count_container *container,
	size_t count
);

/**
 * @brief Clear the count container
 * 
 * @param container Container to clear
 * @return t_returncode 
 */
t_returncode	count_container_clear(
	struct s_count_container *container
);

HEADER_STATIC_CONST
size_t	count_container_get_count(
	struct s_count_container *container
)
{
	return (container->card_count);
}

#endif
