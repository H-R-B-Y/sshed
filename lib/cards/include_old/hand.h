
#ifndef HAND_H
# define HAND_H

# include "cards.h"
# include "card_plane.h"

# include <notcurses/notcurses.h>


/// @brief Card hand area dimensions and position macros
# define HAND_AREA_X(width, height) (0)
# define HAND_AREA_Y(width, height) (height - (CARD_HEIGHT + 3))
# define HAND_AREA_WIDTH(width, height) (width)
# define HAND_AREA_HEIGHT(width, height) (CARD_HEIGHT + 3)

/// @brief Size of the hand arena allocator (NO LONGER USED)
# define HAND_ARENA_ALLOCATOR_SIZE 54 * (sizeof(struct s_card_plane) + sizeof(t_list) + sizeof(t_free_list_node)) \
		+ 6 * (sizeof(struct s_card_plane) + sizeof(t_list) + sizeof(t_free_list_node))


struct s_deck;
struct notcurses;
struct s_card_plane;

enum e_hand_status
{
	/// @brief Displaying the player's hand
	HAND_DISPLAY_HAND,
	/// @brief Displaying the player's shed
	HAND_DISPLAY_SHED,
	/// @brief Count of hand display types
	HAND_DISPLAY_TYPE_COUNT
};
/**
 * @brief Structure representing a player's hand of cards
 * 
 * The hand contains the player's cards and manages their display and state.
 */
struct s_hand
{
	/// @brief Current status of the hand (showing hand or shed)
	enum e_hand_status	status;
	/// @brief Last status of the hand (for tracking changes)
	enum e_hand_status	last_status;
	/// @brief Whether the hand needs to be re-rendered
	int					hand_dirty;
	/// @brief Linked list of cards in the hand (contains struct s_card_plane)
	t_list				*cards;
	/// @brief Number of cards in the hand
	unsigned int		card_count;
	/// @brief Selected card indices: [0] = hand index, [1] = shed index
	int					card_selected[2]; // selected hand and shed
	/// @brief Maximum number of cards that can be displayed in the hand area
	unsigned int		max_cards_to_display; 
	/// @brief Array of pointers to cards in the shed (first 3 face down, last 3 face up)
	struct s_card_plane *shed[6];
	/// @brief Number of cards in the shed (not to be used for iteration)
	t_u8				shed_count;
	/// @brief Ncplane for the hand area
	struct ncplane		*hand_plane;
	/// @brief Currently selected card plane
	struct s_card_plane	*selected_card_plane;
};

/**
 * @brief Create a new hand
 * @param hand Pointer to store the created hand
 * @param parent Parent ncplane
 * @return 0 on success, 1 on error
 */
int		hand_create(struct s_hand **hand, struct ncplane *parent);

/**
 * @brief Destroy a hand
 * @param hand The hand to destroy
 */
void	hand_destroy(struct s_hand *hand);

/**
 * @brief Add a card to the hand.
 * Those marked with an underscore do not re-render the hand after modification.
 * @param hand The hand to add the card to
 * @param card_desc The card description to add
 * @return 0 on success, 1 on error
 */
int		_hand_add_card(struct s_hand *hand, t_card_desc card_desc);
int		hand_add_card(struct s_hand *hand, t_card_desc card_desc);

/**
 * @brief Remove a card from the hand.
 * Those marked with an underscore do not re-render the hand after modification.
 * @param hand The hand to remove the card from
 * @param card_desc The card description to remove
 */
void	_hand_remove_card(struct s_hand *hand, t_card_desc card_desc);
void	hand_remove_card(struct s_hand *hand, t_card_desc card_desc);

/**
 * @brief Add a card to the shed.
 * Those marked with an underscore do not re-render the hand after modification.
 * @param hand The hand to add the card to
 * @param card_desc The card description to add
 * @return 0 on success, 1 on error
 */
int		_hand_add_card_to_shed(struct s_hand *hand, t_card_desc card_desc);
int		hand_add_card_to_shed(struct s_hand *hand, t_card_desc card_desc);

/**
 * @brief Remove a card from the shed.
 * Those marked with an underscore do not re-render the hand after modification.
 * @param hand The hand to remove the card from
 * @param card_desc The card description to remove
 */
void	_hand_remove_card_shed(struct s_hand *hand, t_card_desc card_desc);
void	hand_remove_card_shed(struct s_hand *hand, t_card_desc card_desc);

/**
 * @brief Render the hand
 * @param hand The hand to render
 * @return 0 on success, 1 on error
 */
int		hand_render(struct s_hand *hand);

/**
 * @brief Clear the hand area
 * @param hand The hand to clear
 */
void	hand_clear_screen(struct s_hand *hand);

/**
 * @brief Show the hand or shed
 * @param hand The hand to show
 */
void	hand_show_hand(struct s_hand *hand);

/**
 * @brief Show the shed
 * @param hand The hand to show
 */
void	hand_show_shed(struct s_hand *hand);

/**
 * @brief Toggle between showing the hand and the shed
 * @param hand The hand to toggle
 */
void	hand_toggle_display(struct s_hand *hand);

/*
Selecting cards
*/

/**
 * @brief Get the currently selected card plane
 * @param hand The hand to get the selected card from
 * @return Pointer to the selected card plane
 */
struct s_card_plane	*_hand_get_selected_card_plane(struct s_hand *hand);

/**
 * @brief Select the next card in the hand or shed
 * @param hand The hand to select the next card in
 * @return 0 on success, 1 on error
 */
int					hand_select_next_card(struct s_hand *hand);

/**
 * @brief Select the previous card in the hand or shed
 * @param hand The hand to select the previous card in
 * @return 0 on success, 1 on error
 */
int					hand_select_prev_card(struct s_hand *hand);

/**
 * @brief Update the selected card plane based on the current selection indices
 * @param hand The hand to update the selected card in
 * @return 0 on success, 1 on error
 */
int					hand_update_selected(struct s_hand *hand);

/**
 * @brief Pop the currently selected card from the hand or shed
 * @param hand The hand to pop the selected card from
 * @param popped_card Pointer to store the popped card description
 * @return 0 on success, 1 on error
 */
int					hand_pop_selected_card(struct s_hand *hand, struct s_card_desc *popped_card);

/**
 * @brief Debug function to print the contents of the shed
 * @param hand The hand whose shed to print
 */
void	debug_print_shed(struct s_hand *hand);


/**
 * @brief Return all cards from the hand to the deck
 * @param hand The hand to return cards from
 * @param deck The deck to return cards to
 * @return 0 on success, 1 on error
 */
int	hand_return_to_deck(
	struct s_hand *hand,
	struct s_deck *deck
);

/**
 * @brief Calculate the maximum number of cards that can be displayed in the hand area
 * @param stdplane The standard ncplane
 * @param hand The hand to calculate for
 * @return Maximum number of displayable cards
 */
int	hand_calculate_max_displayable(
	struct ncplane *stdplane,
	struct s_hand *hand
);

/**
 * @brief Check if the hand contains a specific card
 * @param hand The hand to check
 * @param card_desc The card description to check for
 * @return 1 if the card is in the hand, 0 otherwise
 */
int	hand_contains_card(
	struct s_hand *hand,
	struct s_card_desc card_desc
);

/**
 * @brief Check if a specific card is in the shed
 * @param hand The hand to check
 * @param card_desc The card description to check for
 * @return 1 if the card is in the shed, 0 otherwise
 */
int	hand_card_in_shed(
	struct s_hand *hand,
	struct s_card_desc card_desc
);

/**
 * @brief Check if a specific card is in the hand or shed
 * @param hand The hand to check
 * @param card_desc The card description to check for
 * @return 1 if the card is in the hand or shed, 0 otherwise
 */
int	hand_card_in_hand(
	struct s_hand *hand,
	struct s_card_desc card_desc
);

#endif
