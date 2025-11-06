
#ifndef DECK_DISPLAY_H
# define DECK_DISPLAY_H

# include "deck.h"
# include "card_plane.h"
# include <notcurses/notcurses.h>

struct s_pdisplay;
struct s_hand;

/**
 * @brief Structure representing a deck display
 * 
 * Deck display shows the top card of the deck along with a counter for how many cards
 * are in the deck.
 */
struct s_deck_display
{
	/// @brief Parent ncplane
	struct ncplane	*plane;
	/// @brief Ncplane for the card back visual
	struct ncplane	*visual;
	/// @brief Ncplane for the card count display
	unsigned int	x;
	/// @brief Y position of the deck display
	unsigned int	y;
	
	/// @brief The deck being displayed
	struct s_deck	*deck;
	/// @brief Is the deck hidden
	int				hidden;
	/// @brief Is the deck dirty (needs re-render)
	t_u8			is_dirty;
};

/**
 * @brief Initialize a deck display
 * @param deck_display Pointer to store the created deck display
 * @param parent Parent ncplane
 * @param deck The deck to display
 * @param x X position of the deck display
 * @param y Y position of the deck display
 */
int			deck_display_create(
	struct s_deck_display **deck_display,
	struct ncplane *parent,
	struct s_deck *deck,
	unsigned int x,
	unsigned int y
);

/**
 * @brief Destroy a deck display
 * @param deck_display The deck display to destroy
 */
void		deck_display_destroy(
	struct s_deck_display *deck_display
);

/**
 * @brief Render the deck display
 * @param deck_display The deck display to render
 * @return 0 on success, 1 on error
 */
int			deck_display_render(
	struct s_deck_display *deck_display
);

/**
 * @brief Hide the deck display
 * @param deck_display The deck display to hide
 * @return 0 on success, 1 on error
 */
int			deck_display_hide(
	struct s_deck_display *deck_display
);

/**
 * @brief Show the deck display
 * @param deck_display The deck display to show
 * @return 0 on success, 1 on error
 */
int			deck_display_show(
	struct s_deck_display *deck_display
);

/**
 * @brief Clear the deck display from the screen
 * @param deck_display The deck display to clear
 * @return 0 on success, 1 on error
 */
int			deck_display_clear_screen(
	struct s_deck_display *deck_display
);

/**
 * @brief Toggle the visibility of the deck display
 * @param deck_display The deck display to toggle
 * @return 0 on success, 1 on error
 */
int			deck_display_toggle_visibility(
	struct s_deck_display *deck_display
);

/**
 * @brief Move the deck display to a new position
 * @param deck_display The deck display to move
 * @param new_x New X position
 * @param new_y New Y position
 * @return 0 on success, 1 on error
 */
int			deck_display_move(
	struct s_deck_display *deck_display,
	unsigned int new_x,
	unsigned int new_y
);

/**
 * @brief Pop the top card from the deck display
 * @param deck_display The deck display to draw from
 * @param card Pointer to store the drawn card description
 * @return 0 on success, 1 on error
 */
int	deck_display_draw_top_card(
	struct s_deck_display *deck_display,
	struct s_card_desc	*card
);

/**
 * @brief Peek at the top card of the deck display without removing it
 * @param deck_display The deck display to peek at
 * @param card Pointer to store the peeked card description
 * @return 0 on success, 1 on error
 */
int	deck_display_peek_top_card(
	struct s_deck_display *deck_display,
	struct s_card_desc *card
);

/**
 * @brief Return a card to the deck display
 * @param deck_display The deck display to return the card to
 * @param card_desc The card description to return
 * @return 0 on success, 1 on error
 */
int			return_card_to_deck_display(
	struct s_deck_display *deck_display,
	t_card_desc card_desc
);

/**
 * @brief Draw a number of cards from the deck display to a hand or pdisplay
 * @param deck_display The deck display to draw from
 * @param is_pdisplay true if drawing to a pdisplay, false if drawing to a hand
 * @param hand Pointer to the hand or pdisplay to draw to
 * @param count Number of cards to draw
 * @return number of cards drawn to hand
 */
int	deck_display_draw_to_hand(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *hand,
	unsigned int count
);

/**
 * @brief Draw a number of cards from the deck display to a shed
 * @param deck_display The deck display to draw from
 * @param is_pdisplay true if drawing to a pdisplay, false if drawing to a hand
 * @param shed Pointer to the shed of the hand or pdisplay to draw to
 * @param count Number of cards to draw
 * @return number of cards drawn to shed
 */
int deck_display_draw_to_shed(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *shed,
	unsigned int count
);

/**
 * @brief Check if the deck display is empty
 * @param deck_display The deck display to check
 * @return 1 if empty, 0 if not
 */
static inline int	deck_display_is_empty(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (1);
	return (deck_display->deck->remaining == 0);
}

/**
 * @brief Get the number of remaining cards in the deck display
 * @param deck_display The deck display to check
 * @return Number of remaining cards
 */
static inline unsigned int	deck_display_remaining_cards(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (0);
	return (deck_display->deck->remaining);
}

#endif
