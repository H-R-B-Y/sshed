
#ifndef PILE_DISPLAY_H
# define PILE_DISPLAY_H

# include <notcurses/notcurses.h>
# include "card_plane.h"

/*
So a pile represents a stack of cards
they can be face up or face down, and display in a stacked manner.
We want all of these variables to be changeable at runtime so we need
to factor that into how we design this.
Since we can only display one visual per ncplane, we will need to create
a ncplane for each card up to max_stack
*/

enum e_pile_display_orientation
{
	/// @brief Horizontal pile display
	PILE_DISPLAY_HORIZONTAL,
	/// @brief Vertical pile display
	PILE_DISPLAY_VERTICAL,
	/// @brief Count of pile display orientations
	PILE_DISPLAY_ORIENTATION_COUNT
};

/*
Or would it be better to use cdll of card_planes?
card_planes we can already manipulate the orientation, and
we can just hide them when not in use.
Yeah this seems a better idea.
*/
struct s_pile_display
{
	/// @brief Orientation of the pile display
	enum e_pile_display_orientation		orientation;
	/// @brief Orientation of the cards in the pile
	enum e_card_orientation				card_orientation;
	/// @brief X position of the pile display
	unsigned int						x;
	/// @brief Y position of the pile display
	unsigned int						y;
	/// @brief Padding between cards in the pile
	unsigned int						padding;
	/// @brief Ncplane for the pile display area
	struct ncplane						*plane;
	/// @brief Whether the pile cards are face down
	t_u8								is_face_down;
	/// @brief Circular doubly linked list of cards in the pile (contains struct s_card_plane)
	t_cdll								*cards; // circular doubly linked list of card_planes
	/// @brief Maximum number of cards to display in the pile
	unsigned int						max_stack;
	/// @brief Whether the pile display is visible
	int									is_visible;
	/// @brief Whether the pile display is dirty and needs to be redrawn
	t_u8								is_dirty;
	ssize_t								selected;
};

/**
 * @brief Create a pile display
 * 
 * TODO: simplify this, it takes too many parameters right now
 * 
 * @param pile_display Pointer to store the created pile display
 * @param parent Parent ncplane
 * @param padding Padding between cards in the pile
 * @param card_orientation Orientation of the cards in the pile
 * @param orientation Orientation of the pile display
 * @param max_stack Maximum number of cards to display in the pile
 * @param x X position of the pile display
 * @param y Y position of the pile display
 * @return 0 on success, 1 on error
 */
int		pile_display_create(
	struct s_pile_display **pile_display,
	struct ncplane *parent,
	unsigned int padding,
	enum e_card_orientation card_orientation,
	enum e_pile_display_orientation orientation,
	unsigned int max_stack,
	unsigned int x,
	unsigned int y
);

/**
 * @brief Destroy a pile display
 * @param pile_display The pile display to destroy
 */
void	pile_display_destroy(
	struct s_pile_display *pile_display
);

/**
 * @brief Render the pile display
 * @param pile_display The pile display to render
 * @return 0 on success, 1 on error
 */
int		pile_display_render(
	struct s_pile_display *pile_display
);

/**
 * @brief Add a card to the top of the pile display
 * @param pile_display The pile display to add the card to
 * @param card_desc The card description to add
 * @return 0 on success, 1 on error
 */
int		pile_display_add_card_top(
	struct s_pile_display *pile_display,
	t_card_desc card_desc
);

/**
 * @brief Add a card to the bottom of the pile display
 * @param pile_display The pile display to add the card to
 * @param card_desc The card description to add
 * @return 0 on success, 1 on error
 */
int		pile_display_add_card_bottom(
	struct s_pile_display *pile_display,
	t_card_desc card_desc
);

/**
 * @brief Remove a card from the pile display
 * @param pile_display The pile display to remove the card from
 * @param card_desc The card description to remove
 * @return 0 on success, 1 on error
 */
int		pile_display_remove_card(
	struct s_pile_display *pile_display,
	t_card_desc card_desc
);

/**
 * @brief Clear the pile display screen
 * @param pile_display The pile display to clear
 * @return 0 on success, 1 on error
 */
int pile_display_clear_screen(
	struct s_pile_display *pile_display
);

/**
 * @brief Toggle the visibility of the pile display
 * @param pile_display The pile display to toggle
 * @return 0 on success, 1 on error
 */
int pile_display_toggle_visibility(
	struct s_pile_display *pile_display
);

/**
 * @brief Move the pile display to a new position
 * @param pile_display The pile display to move
 * @param new_x The new X position
 * @param new_y The new Y position
 * @return 0 on success, 1 on error
 */
int pile_display_move(
	struct s_pile_display *pile_display,
	unsigned int new_x,
	unsigned int new_y
);

/**
 * @brief Get the top card of the pile display
 * @param pile_display The pile display to get the top card from
 * @param card Pointer to store the top card description
 * @return 0 on success, 1 on error
 */
int	pile_top_card(
	struct s_pile_display *pile_display,
	struct s_card_desc *card
);

/**
 * @brief Get the bottom card of the pile display
 * @param pile_display The pile display to get the bottom card from
 * @param card Pointer to store the bottom card description
 * @return 0 on success, 1 on error
 */
int	pile_bottom_card(
	struct s_pile_display *pile_display,
	struct s_card_desc *card
);

/**
 * @brief Return all cards from the pile display to the hand or pdisplay
 * @param pile_display The pile display to return cards from
 * @param is_pdisplay Whether the destination is a pdisplay (1) or hand (0)
 * @param hand Pointer to the hand or pdisplay to return cards to
 * @return 0 on success, 1 on error
 */
int	pile_display_return_to_hand(
	struct s_pile_display *pile_display,
	int is_pdisplay,
	void *hand
);


// TODO: implement the following
// int pile_display_set_face_down(
// 	struct s_pile_display *pile_display,
// 	t_u8 is_face_down
// );

// int pile_display_set_padding(
// 	struct s_pile_display *pile_display,
// 	unsigned int padding
// );

// int pile_display_set_max_stack(
// 	struct s_pile_display *pile_display,
// 	unsigned int max_stack
// );

// int pile_display_set_card_orientation(
// 	struct s_pile_display *pile_display,
// 	enum e_card_orientation card_orientation
// );

/**
 * @brief Check if the pile display is empty
 * @param pile_display The pile display to check
 * @return 1 if the pile display is empty, 0 otherwise
 */
inline int	pile_display_is_empty(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return (1);
	return (pile_display->cards->count == 0);
}

/**
 * @brief Get the count of cards in the pile display
 * @param pile_display The pile display to get the count from
 * @return The count of cards in the pile display, or -1 on error
 */
inline unsigned int	pile_display_get_count(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return (-1);
	return ((unsigned int)(pile_display->cards->count));
}

#endif
