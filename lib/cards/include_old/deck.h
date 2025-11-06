
#ifndef DECK_H
# define DECK_H

# define FT_INCLUDE_ALL
# include "libft.h"
# include "cards.h"

struct s_deck
{
	/// @brief Memory arena for the deck cards
	void				*arena;
	/// @brief Array of pointers to card descriptions in the deck
	struct s_card_desc	*cards[55];
	/// @brief Number of cards currently remaining in the deck
	int					has_jokers;
	/// @brief Number of cards currently remaining in the deck
	int					remaining;
};

/**
 * @brief Structure representing a deck of cards
 * Deck contains a collection of cards and manages their state.
 * @param include_jokers Whether to include jokers in the deck
 * @return Pointer to the created deck
 */
struct s_deck	*deck_create(int include_jokers);

/**
 * @brief Destroy a deck of cards
 * @param deck The deck to destroy
 */
void			deck_destroy(struct s_deck *deck);

/**
 * @brief Shuffle the deck of cards
 * @param deck The deck to shuffle
 * @param seed Seed for the random number generator
 * @return 0 on success, 1 on error
 */
int				deck_shuffle(struct s_deck *deck, unsigned int seed);

/**
 * @brief Draw a card from the deck
 * @param deck The deck to draw from
 * @param card Pointer to store the drawn card description
 * @return 0 on success, 1 on error
 */
int				deck_draw_card(struct s_deck *deck, struct s_card_desc *card);

/**
 * @brief Peek at a card in the deck without removing it
 * @param deck The deck to peek at
 * @param index Index of the card to peek at (0 = top card)
 * @param card Pointer to store the peeked card description
 * @return 0 on success, 1 on error
 */
int				deck_peek_card(struct s_deck *deck, unsigned int index, struct s_card_desc *card);

/**
 * @brief Return a card to the deck
 * @param deck The deck to return the card to
 * @param card The card description to return
 */
void			deck_return_card(struct s_deck *deck, struct s_card_desc card);

#endif

