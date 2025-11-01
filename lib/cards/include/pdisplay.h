
#ifndef PDISPLAY_H
# define PDISPLAY_H

#include "card_plane.h"

/*
PDisplay is responsible for displaying the hands of other players
(non-player hands) aroudn the table.

at most 3 can be displayed at once, one on each side of the player.

Each PDisplay will have a plane associated with it, and each PDisplay
will have a set of cards in it, we dont need to know the actual cards,
(but we do for a local game i guess) but we need to know how many cards
are in the hand, and how many cards are in the shed (both face up and face down)
*/

/// @brief If we are left or right, the width is the height of the card with margin, else its the full width of the parent
# define PDISPLAY_AREA_WIDTH(orientation, parent_width, parent_height) \
	((orientation == PDISPLAY_ORIENTATION_LEFT || orientation == PDISPLAY_ORIENTATION_RIGHT) ? \
		(CARD_H_WIDTH + 3) : (parent_width))

/// @brief If we are top or bottom, the height is the height of the card with margin, else its the full height of the parent
# define PDISPLAY_AREA_HEIGHT(orientation, parent_width, parent_height) \
	((orientation == PDISPLAY_ORIENTATION_LEFT || orientation == PDISPLAY_ORIENTATION_RIGHT) ? \
		(parent_height) : (CARD_HEIGHT + 2))

/// @brief X position based on orientation
/// If left, x = 0
/// If right x = parent_width - card height - 1
/// If top then x = 0
# define PDISPLAY_AREA_X(orientation, parent_width, parent_height) \
	((orientation == PDISPLAY_ORIENTATION_LEFT) ? 0 : \
	(orientation == PDISPLAY_ORIENTATION_RIGHT) ? (parent_width - (CARD_H_WIDTH + 3)) : 0)

/// @brief Y position is just 0 because all the planes are aligned to the top
# define PDISPLAY_AREA_Y(orientation, parent_width, parent_height) 0


enum e_pdisplay_orientation
{
	/// @brief Left side of the player
	PDISPLAY_ORIENTATION_LEFT,
	/// @brief Right side of the player
	PDISPLAY_ORIENTATION_RIGHT,
	/// @brief Top side of the player
	PDISPLAY_ORIENTATION_TOP,
	/// @brief Count of pdisplay orientations
	PDISPLAY_ORIENTATION_COUNT
};

enum e_pdisplay_status
{
	/// @brief Displaying the player's hand
	PDISPLAY_HAND,
	/// @brief Displaying the player's shed
	PDISPLAY_SHED,
	/// @brief Count of pdisplay types
	PDISPLAY_TYPE_COUNT
};

static const char *pdisplay_orientation_str[PDISPLAY_ORIENTATION_COUNT]
__attribute__((unused)) = {
	[PDISPLAY_ORIENTATION_LEFT] = "PDISPLAY_LEFT",
	[PDISPLAY_ORIENTATION_RIGHT] = "PDISPLAY_RIGHT",
	[PDISPLAY_ORIENTATION_TOP] = "PDISPLAY_TOP",
};

struct s_pdisplay
{
	/// @brief Current status of the pdisplay (showing hand or shed)
	enum e_pdisplay_status		status;
	/// @brief Orientation of the pdisplay
	enum e_pdisplay_orientation	orientation;
	/// @brief Ncplane for the pdisplay area
	struct ncplane				*plane;
	/// @brief Maximum number of cards that can be displayed in the pdisplay area
	unsigned int				max_cards_to_display;
	/// @brief Number of cards in the pdisplay hand
	t_u8						card_count;
	/// @brief Number of cards in the pdisplay shed
	t_u8						shed_count;
	/// @brief Linked list of cards in the pdisplay hand (contains struct s_card_plane)
	t_list						*cards;
	/// @brief Array of pointers to cards in the shed (first 3 face down, last 3 face up)
	struct s_card_plane			*shed[6];
	/// @brief Whether the pdisplay needs to be re-rendered
	t_u8						pdisplay_dirty;
};

/**
 * @brief Create a pdisplay for an AI player
 * @param pdisplay Pointer to store the created pdisplay
 * @param parent Parent ncplane
 * @param orientation Orientation of the pdisplay
 * @return 0 on success, 1 on error
 */
int		pdisplay_create(
	struct s_pdisplay **pdisplay,
	struct ncplane *parent,
	enum e_pdisplay_orientation orientation
);

/**
 * @brief Destroy a pdisplay
 * @param pdisplay The pdisplay to destroy
 */
void	pdisplay_destroy(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Render the pdisplay
 * @param pdisplay The pdisplay to render
 * @return 0 on success, 1 on error
 */
int		pdisplay_render(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Clear the pdisplay area
 * @param pdisplay The pdisplay to clear
 * @return 0 on success, 1 on error
 */
int		pdisplay_clear_screen(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Show the pdisplay hand
 * @param pdisplay The pdisplay to show
 * @return 0 on success, 1 on error
 */
int pdisplay_show_hand(struct s_pdisplay *pdisplay);

/**
 * @brief Show the pdisplay shed
 * @param pdisplay The pdisplay to show
 * @return 0 on success, 1 on error
 */
int pdisplay_show_shed(struct s_pdisplay *pdisplay);

/**
 * @brief Toggle between showing the pdisplay hand and shed
 * @param pdisplay The pdisplay to toggle
 * @return 0 on success, 1 on error
 */
int pdisplay_toggle_display(struct s_pdisplay *pdisplay);

/*
Adding and removing cards from the pdisplay 
these are the generics for a local game where we know the actual cards
*/

/**
 * @brief Add a card to the pdisplay hand
 * Those marked with an underscore do not re-render the pdisplay after modification.
 * @param pdisplay The pdisplay to add the card to
 * @param card_desc The card description to add
 * @return 0 on success, 1 on error
 */
int	_pdisplay_add_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);
int	pdisplay_add_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);

/**
 * @brief Remove a card from the pdisplay hand
 * Those marked with an underscore do not re-render the pdisplay after modification.
 * @param pdisplay The pdisplay to remove the card from
 * @param card_desc The card description to remove
 * @return 0 on success, 1 on error
 */
int	_pdisplay_remove_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);
int	pdisplay_remove_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);

/**
 * @brief Add a card to the pdisplay shed
 * Those marked with an underscore do not re-render the pdisplay after modification.
 * @param pdisplay The pdisplay to add the card to
 * @param card_desc The card description to add
 * @return 0 on success, 1 on error
 */
int	_pdisplay_add_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);
int	pdisplay_add_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);

/**
 * @brief Remove a card from the pdisplay shed
 * Those marked with an underscore do not re-render the pdisplay after modification.
 * @param pdisplay The pdisplay to remove the card from
 * @param card_desc The card description to remove
 * @return 0 on success, 1 on error
 */
int	_pdisplay_remove_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);
int	pdisplay_remove_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
);

/*
Then we have the generics for an online game where we dont know the actual cards
*/

/**
 * @brief Add an unknown card to the pdisplay hand
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to add the card to
 * @return 0 on success, 1 on error
 */
int	_pdisplay_add_unknown_card(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Remove an unknown card from the pdisplay hand
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to remove the card from
 * @return 0 on success, 1 on error
 */
int	pdisplay_add_unknown_card(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Remove an unknown card from the pdisplay hand
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to remove the card from
 * @return 0 on success, 1 on error
 */
int	_pdisplay_remove_unknown_card(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Remove an unknown card from the pdisplay hand
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to remove the card from
 * @return 0 on success, 1 on error
 */
int	pdisplay_remove_unknown_card(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Add an unknown card to the pdisplay shed
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to add the card to
 * @return 0 on success, 1 on error
 */
int	_pdisplay_add_unknown_card_shed(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Add an unknown card to the pdisplay shed
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to add the card to
 * @return 0 on success, 1 on error
 */
int	pdisplay_add_unknown_card_shed(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Remove an unknown card from the pdisplay shed
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to remove the card from
 * @return 0 on success, 1 on error
 */
int	_pdisplay_remove_unknown_card_shed(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Remove an unknown card from the pdisplay shed
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to remove the card from
 * @return 0 on success, 1 on error
 */
int	pdisplay_remove_unknown_card_shed(
	struct s_pdisplay *pdisplay
);

/**
 * @brief Pop a specific card from the pdisplay hand
 * @param pdisplay The pdisplay to pop the card from
 * @param card_desc The card description to pop
 * @return 0 on success, 1 on error
 */
int	pdisplay_pop_card_desc(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
);

/**
 * @brief Pop a card at a specific index from the pdisplay hand
 * This needs to be thought through more carefully so dont use this for now
 * @param pdisplay The pdisplay to pop the card from
 * @param index The index of the card to pop
 * @param popped_card Pointer to store the popped card description
 * @return 0 on success, 1 on error
 */
int	pdisplay_pop_card_index(
	struct s_pdisplay *pdisplay,
	int index,
	struct s_card_desc *popped_card
);

/**
 * @brief Calculate the maximum number of cards that can be displayed in the pdisplay area
 * @param stdplane The standard ncplane
 * @param pdisplay The pdisplay to calculate for
 * @return 0 on success, 1 on error
 */
int	pdisplay_calculate_max_displayable(
	struct ncplane *stdplane,
	struct s_pdisplay *pdisplay
);

// Checks

/**
 * @brief Check if the pdisplay has a specific card
 * @param pdisplay The pdisplay to check
 * @param card_desc The card description to check for
 * @return 1 if the card is in the pdisplay, 0 otherwise
 */
int	pdisplay_has_card(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
);

/**
 * @brief Check if a specific card is in the pdisplay shed
 * @param pdisplay The pdisplay to check
 * @param card_desc The card description to check for
 * @return 1 if the card is in the shed, 0 otherwise
 */
int	pdisplay_card_is_shed(
	struct s_pdisplay *pdispay,
	struct s_card_desc card_desc
);

/**
 * @brief Check if a specific card is in the pdisplay hand
 * @param pdisplay The pdisplay to check
 * @param card_desc The card description to check for
 * @return 1 if the card is in the hand, 0 otherwise
 */
int	pdisplay_card_is_hand(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
);

#endif
