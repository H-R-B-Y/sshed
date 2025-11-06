/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shed_container.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:31:25 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 16:48:58 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHED_CONTAINER_H
# define SHED_CONTAINER_H

# include "core/card.h"

// Shed container is really specific but I am going to use it so 
// I think its worth making it its own contianer type
// A shed is defined as a collection of cards that
// have both a face up list and face down list
// Not sure if it is better to have two separate containers or one container with flags

/*
In the game you cannot play down cards from the shed, until all the face up cards are played
Do i build that into the logic here? or do i leave that to the game logic?


And how to we remove cards from the shed, like the face down cards, from the game interface
how do we address them. I thought using a index would be best but not sure
*/

/**
 * @brief A slot for a card in the shed container
 */
struct s_card_slot
{
	/// @brief The card in this slot
	struct s_card_desc	card;
	/// @brief Whether this slot is filled
	t_u8				is_filled;
};

/**
 * @brief Shed container with face-up and face-down card slots
 */
struct s_shed_container
{
	/// @brief Capacity of each half (face-up and face-down)
	t_u8				half_capacity;
	/// @brief Array of face-down card slots
	struct s_card_slot	*face_down;
	/// @brief Number of filled face-down slots
	size_t				face_down_count;
	/// @brief Array of face-up card slots
	struct s_card_slot	*face_up;
	/// @brief Number of filled face-up slots
	size_t				face_up_count;
};

/**
 * @brief Initialize a shed container
 * 
 * @param shed Pointer to the shed container
 * @param half_capacity Half the capacity of the shed (face up and face down each get this many slots)
 * @return t_returncode 
 */
t_returncode	shed_container_init(
	struct s_shed_container *shed,
	t_u8 half_capacity
);

/**
 * @brief Free a shed container
 * 
 * @param shed The shed container to free
 * @return * void 
 */
void			shed_container_free(
	struct s_shed_container *shed
);

/**
 * @brief Add a face down card to the shed at index
 * 
 * @param shed Shed container
 * @param card Card to add
 * @param index Index to add the card at
 * @return t_returncode 
 */
t_returncode	shed_add_face_down(
	struct s_shed_container *shed,
	struct s_card_desc card,
	size_t index
);

/**
 * @brief Add a face up card to the shed at index
 * 
 * @param shed Shed container
 * @param card Card to add
 * @param index Index to add the card at
 * @return t_returncode 
 */
t_returncode	shed_add_face_up(
	struct s_shed_container *shed,
	struct s_card_desc card,
	size_t index
);

/**
 * @brief Add a card to the shed, automatically choosing face up or face down
 * 
 * @param shed Shed container
 * @param card Card to add
 * @param index [out] Index the card was added at
 * @param face_down [out] Face down or face up
 * @return t_returncode 
 */
t_returncode	shed_add_card(
	struct s_shed_container *shed,
	struct s_card_desc card,
	size_t *index,
	int *face_down
);

/**
 * @brief Remove a face down card from the shed at index
 * 
 * @param shed Shed container
 * @param index Index to remove the card from
 * @param removed_card [out] Pointer to store the removed card
 * @return t_returncode 
 */
t_returncode	shed_remove_face_down(
	struct s_shed_container *shed,
	size_t index,
	struct s_card_desc *removed_card
);

/**
 * @brief Remove a face up card from the shed at index
 * 
 * @param shed Shed container
 * @param index Index to remove the card from
 * @param removed_card [out] Pointer to store the removed card
 * @return t_returncode 
 */
t_returncode	shed_remove_face_up(
	struct s_shed_container *shed,
	size_t index,
	struct s_card_desc *removed_card
);

/**
 * @brief Remove a card from the shed at index, specifying face up or face down
 * 
 * @param shed Shed container
 * @param index Index to remove the card from
 * @param face_down Whether to remove from face down or face up
 * @param removed_card [out] Pointer to store the removed card
 * @return t_returncode 
 */
t_returncode	shed_remove_card(
	struct s_shed_container *shed,
	size_t index,
	int face_down,
	struct s_card_desc *removed_card
);

/**
 * @brief Clear all cards from the shed
 * 
 * @param shed Shed container
 * @return t_returncode 
 */
t_returncode	shed_clear(
	struct s_shed_container *shed
);

/**
 * @brief Get a face down card from the shed at index
 * 
 * @param shed Shed container
 * @param index Index to get the card from
 * @param face_down Flag to indicate face down
 * @param out_card [out] Pointer to store the retrieved card
 * @return t_returncode 
 */
t_returncode	shed_get_card(
	struct s_shed_container *shed,
	size_t index,
	int face_down,
	struct s_card_desc *out_card
);

/**
 * @brief Check if the shed has a specific card
 * 
 * @param shed Shed container
 * @param card Card to check for
 * @param face_down [out] Pointer to store if the card is face down (1) or face up (0)
 * @return t_returncode Ok if card is found
 */
int	shed_has_card(
	struct s_shed_container *shed,
	struct s_card_desc card,
	int *face_down
);

/**
 * @brief Iterate over all cards in the shed container and call the callback for each
 * 
 * @param container Container to iterate over
 * @param callback Callback function to call for each card
 * @param user_data User data to pass to the callback
 * @return t_returncode 
 */
t_returncode	shed_container_foreach(
	struct s_shed_container *container,
	int (*callback)(struct s_card_desc card, size_t index, int face_down, void *data),
	void *user_data
);

HEADER_STATIC_CONST size_t	shed_get_face_down_count(
	struct s_shed_container *shed
)
{
	return (shed->face_down_count);
}


#endif
