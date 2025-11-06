/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_set.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:18:52 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:30:17 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CARD_SET_H
# define CARD_SET_H

# define FT_INCLUDE_ALL
# include "libft/libft.h"

# include "card.h"

/*
A card set is the colletion of all cards that can exist
It acts as the central lookup for card instances based on their descriptions
*/

// Do we need this?
// Because this was only really needed when we wanted to lookup the display graphics for a card
// But core is not handling graphics directly so maybe we don't need this here
// But it would be nice to have a central lookup that user can attach user data too when it is needed
// But then how do we load all of the cards into the set with user data
// We are going to need some unique array the user can iterate over to add data
// And later a lookup for all of this data using hashing

struct s_card_set
{
	/// @brief Hashmap for card description to user data
	t_hashmap			card_map;
	/// @brief Array of all card descriptions in the set
	struct s_card_desc	*cards;
	/// @brief Count of cards in the set
	size_t				card_count;
	/// @brief Capacity of the card set
	size_t				capacity;
	/// @brief User data associated with the set
	void				*set_user_data;
	/// @brief Function to free set user data
	t_freefn			free_set_user_data;
	/// @brief Function to free card user data
	t_freefn			free_card_user_data;
};

/**
 * @brief Initialize a card set
 * 
 * @param set Set to initialize
 * @param card_count Count of cards in the set
 * @param set_user_data User data for the set
 * @return t_returncode 
 */
t_returncode	card_set_init(
	struct s_card_set *set,
	size_t card_count,
	void *set_user_data,
	t_freefn free_set_user_data,
	t_freefn free_card_user_data
);

/**
 * @brief Free a card set
 * 
 * @param set Set to free
 */
void			card_set_free(
	struct s_card_set *set
);

/**
 * @brief Add a card to the card set
 * 
 * @param set Set to add the card to
 * @param card_desc Card description to add
 * @param card_user_data User data associated with the card
 * @return t_returncode 
 */
t_returncode	card_set_add_card(
	struct s_card_set *set,
	struct s_card_desc card_desc,
	void *card_user_data
);

/**
 * @brief Get user data for a card in the set
 * 
 * @param set Set to lookup the card in
 * @param card_desc Card description to lookup
 * @return void* 
 */
void			*card_set_get_card_data(
	struct s_card_set *set,
	struct s_card_desc card_desc
);


#endif
