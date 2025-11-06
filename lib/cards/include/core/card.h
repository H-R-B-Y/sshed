/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 14:11:49 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 14:26:32 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CARD_H
# define CARD_H

# define FT_INCLUDE_ALL
# include "libft/libft.h"

# ifndef HEADER_STATIC_CONST
#  ifndef __DOXYGEN__
#   define HEADER_STATIC_CONST static const __attribute__((unused))
#  else
#   define HEADER_STATIC_CONST static const
#  endif
# endif

enum e_suit
{
	/// @brief Clubs suit
	SUIT_CLUBS,
	/// @brief Diamonds suit
	SUIT_DIAMONDS,
	/// @brief Spades suit
	SUIT_SPADES,
	/// @brief Hearts suit
	SUIT_HEARTS,
	/// @brief Joker suit, Special use case
	SUIT_JOKER,
	/// @brief Count of suits
	SUIT_COUNT
};

enum e_rank
{
	/// @brief Ace rank
	RANK_ACE,
	/// @brief Two rank
	RANK_TWO,
	/// @brief Three rank
	RANK_THREE,
	/// @brief Four rank
	RANK_FOUR,
	/// @brief Five rank
	RANK_FIVE,
	/// @brief Six rank
	RANK_SIX,
	/// @brief Seven rank
	RANK_SEVEN,
	/// @brief Eight rank
	RANK_EIGHT,
	/// @brief Nine rank
	RANK_NINE,
	/// @brief Ten rank
	RANK_TEN,
	/// @brief Jack rank
	RANK_JACK,
	/// @brief Queen rank
	RANK_QUEEN,
	/// @brief King rank
	RANK_KING,
	/// @brief Joker ranks
	RANK_JOKER_BLACK,
	RANK_JOKER_RED,
	/// @brief Count of ranks
	RANK_COUNT
};

/// @brief String representations of suits and ranks
HEADER_STATIC_CONST char *suit_str[SUIT_COUNT] = {
	[SUIT_CLUBS] = "CLUBS",
	[SUIT_DIAMONDS] = "DIAMONDS",
	[SUIT_SPADES] = "SPADES",
	[SUIT_HEARTS] = "HEARTS",
	[SUIT_JOKER] = "JOKER",
};

/// @brief Short string representations of suits for compact display
HEADER_STATIC_CONST char *rank_str[RANK_COUNT] = {
	"ACE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", 
	"EIGHT", "NINE", "TEN", "JACK", "QUEEN", "KING",
	"JOKER_BLACK", "JOKER_RED"
};

struct s_card_desc
{
	// Define card properties here
	// This should not depend on anything outside of this file
	// it should be build in a way that the user can modify later
	// to build different types of cards
	// but right now it can be for playing cards
	/// @brief Rank of the card
	enum e_rank	rank;
	/// @brief Suit of the card
	enum e_suit	suit;
};

HEADER_STATIC_CONST unsigned int	hash_card(struct s_card_desc *card_desc)
{
	// For jokers, we use a special calculation
	if (card_desc->rank == RANK_JOKER_BLACK)
		return (SUIT_COUNT * RANK_COUNT + 0); // Unique position for black joker
	if (card_desc->rank == RANK_JOKER_RED)
		return (SUIT_COUNT * RANK_COUNT + 1); // Unique position for red joker
	
	// For regular cards, use the normal hash
	return ((unsigned int)card_desc->suit * RANK_COUNT + (unsigned int)card_desc->rank);
}
HEADER_STATIC_CONST int			compare_card(struct s_card_desc *a, struct s_card_desc *b)
{
	return (
	a->rank == b->rank && a->suit == b->suit ? 0 : 1
	);
}

#endif
