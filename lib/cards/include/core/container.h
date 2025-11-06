/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   container.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:05:26 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 17:07:24 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTAINER_H
# define CONTAINER_H

# include "core/containers/array_container.h"
# include "core/containers/list_container.h"
# include "core/containers/shed_container.h"
# include "core/containers/count_container.h"

/**
 * @brief Types of containers available
 */
enum e_container_type
{
	/// @brief Dynamic array container
	CONTAINER_TYPE_ARRAY,
	/// @brief Linked list container
	CONTAINER_TYPE_LIST,
	/// @brief Shed container (face-up/face-down)
	CONTAINER_TYPE_SHED,
	/// @brief Count-only container (for unknown cards)
	CONTAINER_TYPE_COUNT_ONLY,
	CONTAINER_TYPE_COUNT
};

HEADER_STATIC_CONST char *container_type_str[CONTAINER_TYPE_COUNT] = {
	[CONTAINER_TYPE_ARRAY] = "ARRAY",
	[CONTAINER_TYPE_LIST] = "LIST",
	[CONTAINER_TYPE_SHED] = "SHED",
	[CONTAINER_TYPE_COUNT_ONLY] = "COUNT_ONLY",
};

/**
 * @brief Union holding container-specific data
 */
union u_container_data
{
	/// @brief Array container data
	struct s_array_container	array_container;
	/// @brief List container data
	struct s_list_container	list_container;
	/// @brief Shed container data
	struct s_shed_container	shed_container;
	/// @brief Count-only container data
	struct s_count_container	count_container;
};

/**
 * @brief Generic container wrapping different container types
 */
struct s_container
{
	/// @brief Container type
	enum e_container_type	type;
	/// @brief Container-specific data
	union u_container_data	data;
};

/**
 * @brief Callback function for filtering cards
 * 
 * @param card The card to evaluate
 * @param user_data User-provided data for the filter
 * @return Non-zero if card matches filter, 0 otherwise
 */
typedef int (t_card_filter_func)(struct s_card_desc card, void *user_data);

/**
 * @brief Free a container and its resources
 * 
 * @param container Pointer to the container to free
 * @return t_returncode 
 */
t_returncode	container_free(
	struct s_container *container
);

/**
 * @brief Initialize a container of the specified type
 * 
 * @param container Pointer to the container to initialize
 * @param type Container type to create
 * @param initial_capacity Initial capacity (only used for array and shed)
 * @return t_returncode 
 */
t_returncode	container_init(
	struct s_container *container,
	enum e_container_type type,
	size_t initial_capacity
);

/**
 * @brief Move cards from source to destination container based on filter
 * 
 * @param src Source container
 * @param dst Destination container
 * @param filter Filter function to select cards (NULL for all cards)
 * @param user_data User data passed to filter function
 * @return t_returncode 
 */
t_returncode	container_move_cards(
	struct s_container *src,
	struct s_container *dst,
	t_card_filter_func *filter,
	void *user_data
);

/**
 * @brief Transfer all cards from source to destination container
 * 
 * @param src Source container
 * @param dst Destination container
 * @return t_returncode 
 */
t_returncode	container_transfer_all(
	struct s_container *src,
	struct s_container *dst
);

/**
 * @brief Add a card to a container
 * 
 * @param container Pointer to the container
 * @param card Card to add
 * @return t_returncode 
 */
t_returncode	container_add_card(
	struct s_container *container,
	struct s_card_desc card
);

/**
 * @brief Remove a card from a container
 * 
 * @param container Pointer to the container
 * @param card Card to remove
 * @return t_returncode 
 */
t_returncode	container_remove_card(
	struct s_container *container,
	struct s_card_desc card
);

/**
 * @brief Check if a container has a specific card
 * 
 * @param container Pointer to the container
 * @param card Card to check for
 * @return t_returncode Ok if card is found
 */
t_returncode	container_has_card(
	struct s_container *container,
	struct s_card_desc card
);

/**
 * @brief Clear all cards from a container
 * 
 * @param container Pointer to the container
 * @return t_returncode 
 */
t_returncode	container_clear(
	struct s_container *container
);

/**
 * @brief Print container contents (for debugging)
 * 
 * @param container Pointer to the container
 * @return t_returncode 
 */
t_returncode 	container_print(
	struct s_container *container
);

/**
 * @brief Get all cards from a container
 * 
 * @param container Pointer to the container
 * @param out_cards [out] Pointer to store array of cards
 * @param out_count [out] Pointer to store number of cards
 * @return t_returncode 
 */
t_returncode	container_get_all_cards(
	struct s_container *container,
	struct s_card_desc **out_cards,
	size_t *out_count
);

/**
 * @brief Get the number of cards in a container
 * 
 * @param container Pointer to the container
 * @param out_count [out] Pointer to store card count
 * @return t_returncode 
 */
t_returncode	container_get_count(
	struct s_container *container,
	size_t *out_count
);

/**
 * @brief Get filtered cards from a container
 * 
 * @param container Pointer to the container
 * @param filter Filter function to select cards
 * @param user_data User data passed to filter function
 * @param out_cards [out] Pointer to store array of filtered cards
 * @param out_count [out] Pointer to store number of filtered cards
 * @return t_returncode 
 */
t_returncode	container_get_cards(
	struct s_container *container,
	t_card_filter_func *filter,
	void *user_data,
	struct s_card_desc **out_cards,
	size_t *out_count
);



#endif
