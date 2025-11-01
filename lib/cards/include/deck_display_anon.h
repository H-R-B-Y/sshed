
#ifndef DECK_DISPLAY_ANON_H
# define DECK_DISPLAY_ANON_H

# include "cards.h"
# include "card_plane.h"
# include <notcurses/notcurses.h>

/**
 * @brief Structure representing an anonymous deck display
 * 
 * Anonymous deck display shows only the back of a card along with a counter for how many cards
 * are in the deck. It is meant to be used for games where the client does not know 
 * what is in the deck but needs to see how many cards are left.
 */
struct s_deck_anon
{
	/// @brief Parent ncplane
	struct ncplane	*parent;
	/// @brief Ncplane for the deck anon display
	struct ncplane	*plane;
	/// @brief Ncplane for the card back visual
	struct ncplane	*visual;

	/// @brief Is the deck hidden
	t_u8			hidden;
	/// @brief Is the deck dirty (needs re-render)
	t_u8			is_dirty;

	/// @brief Number of cards in the deck
	unsigned int	card_count;
	/// @brief X position of the deck anon
	unsigned int	x;
	/// @brief Y position of the deck anon
	unsigned int	y;
};

/**
 * @brief Initialize a deck anon display
 * @param deck Pointer to store the created deck anon
 * @param parent Parent ncplane
 * @param x X position of the deck anon
 * @param y Y position of the deck anon
 * @param card_count Initial number of cards in the deck
 */
int		deck_anon_init(
		struct s_deck_anon **deck,
		struct ncplane *parent,
		unsigned int x,
		unsigned int y,
		unsigned int card_count
);

/**
 * @brief Destroy a deck anon display
 * @param deck The deck anon to destroy
 * @return 0 on success, 1 on error
 */
void	deck_anon_destroy(
		struct s_deck_anon *deck
);

/**
 * @brief Render the deck anon display
 * @param nc Pointer to the notcurses context
 * @param deck The deck anon to render
 * @return 0 on success, 1 on error
 */
int		deck_anon_render(
		struct notcurses *nc,
		struct s_deck_anon *deck
);

/**
 * @brief Clear the deck anon display from the screen
 * @param deck The deck anon to clear
 * @return 0 on success, 1 on error
 */
int		deck_anon_clear_screen(
		struct s_deck_anon *deck
);

/**
 * @brief Increment the card count in the deck anon
 * @param deck The deck anon to increment
 * @return 0 on success, 1 on error
 */
int		deck_anon_inc(
		struct s_deck_anon *deck
);

/**
 * @brief Decrement the card count in the deck anon
 * @param deck The deck anon to decrement
 * @return 0 on success, 1 on error
 */
int		deck_anon_dec(
		struct s_deck_anon *deck
);

/**
 * @brief Add a number of cards to the deck anon
 * @param deck The deck anon to add cards to
 * @param add Number of cards to add
 * @return 0 on success, 1 on error
 */
int		deck_anon_add(
		struct s_deck_anon *deck,
		unsigned int add
);

/**
 * @brief Subtract a number of cards from the deck anon
 * @param deck The deck anon to subtract cards from
 * @param sub Number of cards to subtract
 * @return 0 on success, 1 on error
 */
int		deck_anon_sub(
		struct s_deck_anon *deck,
		unsigned int sub
);

/**
 * @brief Move the deck anon to a new position
 * @param deck The deck anon to move
 * @param x New X position
 * @param y New Y position
 * @return 0 on success, 1 on error
 */
int		deck_anon_move(
		struct s_deck_anon *deck,
		unsigned int x,
		unsigned int y
);

/**
 * @brief Check if the deck anon is empty
 * @param deck The deck anon to check
 * @return 1 if empty, 0 otherwise
 */
static inline int	deck_anon_is_empty(struct s_deck_anon *deck)
{
	if (!deck)
		return (1);
	return (
		deck->card_count == 0
	);
}

#endif