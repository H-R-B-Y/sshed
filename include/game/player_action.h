/**
 * @file player_action.h
 * @brief Player action system for game moves
 * 
 * This header defines the player action structure and functions for
 * representing and validating player moves in the game.
 * 
 * Actions include:
 * - Playing cards
 * - Swapping cards (during swap phase)
 * - Default action (picking up pile)
 * 
 * The action system is used by both human players and AI to
 * communicate their moves to the game logic.
 */

#ifndef PLAYER_ACTION_H
# define PLAYER_ACTION_H

# include "cards.h"
# include "enum_macro.h"

/* ========================================================================== */
/*                         PLAYER ACTION TYPES                                */
/* ========================================================================== */

/**
 * @brief Player action type enumeration
 * 
 * Defines the types of actions a player can take:
 * - NONE: No action yet selected
 * - SWAP: Swap cards between hand and shed (swap phase only)
 * - PLAY: Play 1-4 cards onto the pile
 * - DEFAULT: Pick up the pile (no valid cards to play)
 */
# define PLAYER_ACTION_LIST(prefix, func) \
	func(prefix, NONE) \
	func(prefix, SWAP) \
	func(prefix, PLAY) \
	func(prefix, DEFAULT)

ENUM_FACTORY(PLAYER_ACTION, player_action, PLAYER_ACTION_LIST);

/* ========================================================================== */
/*                         DATA STRUCTURES                                    */
/* ========================================================================== */

/**
 * @brief Player action structure
 * 
 * Represents a complete player move with all necessary information.
 * This is the primary interface between players/AI and the game logic.
 */
struct s_player_action
{
	/// @brief Type of action (none, swap, play, default)
	t_player_action		action;

	/* Play action data */
	/// @brief Cards to play (1-4 cards for play action)
	struct s_card_desc	cards[4];
	/// @brief Number of cards in the cards array
	t_u8				card_count;

	/* Swap action data */
	/// @brief Cards to swap (paired with hand cards)
	struct s_card_desc	swap_cards[4];
	/// @brief Number of swap cards
	t_u8				swap_card_count;

	/* Action state */
	/// @brief Whether the action is ready to be processed by game logic
	t_u8				ready;
};

/* ========================================================================== */
/*                       ACTION CONSTRUCTION FUNCTIONS                        */
/* ========================================================================== */

/**
 * @brief Create a clean (empty) action structure
 * 
 * Initializes all fields to default values.
 * 
 * @return Clean action structure with action = NONE, ready = 0
 */
struct s_player_action	clean_action(void);

/**
 * @brief Set the action type
 * 
 * Sets the action type and resets card data.
 * Does not validate or populate card data.
 * 
 * @param action The action structure to modify
 * @param type The action type to set
 * @return 0 on success, non-zero on error
 */
int						action_set(
	struct s_player_action *action,
	t_player_action type
);

/**
 * @brief Set play action with cards
 * 
 * Configures the action to play the specified cards.
 * Does not validate card legality - that's done by game logic.
 * 
 * @param action The action structure to modify
 * @param cards Array of card descriptors to play
 * @param card_count Number of cards (1-4)
 * @return 0 on success, non-zero on error
 */
int						action_play_cards(
	struct s_player_action *action,
	struct s_card_desc *cards,
	t_u8 card_count
);

/**
 * @brief Set swap action with card pair
 * 
 * Configures the action to swap a card from the shed with a card from hand.
 * 
 * @param action The action structure to modify
 * @param card_in_shed The card currently in the shed to swap out
 * @param card_in_hand The card currently in hand to swap in
 * @return 0 on success, non-zero on error
 */
int						action_swap_cards(
	struct s_player_action *action, 
	struct s_card_desc *card_in_shed,
	struct s_card_desc *card_in_hand
);

/**
 * @brief Set default action (pick up pile)
 * 
 * Configures the action as a default action (player picks up pile).
 * 
 * @param action The action structure to modify
 * @return 0 on success, non-zero on error
 */
int						action_default_on_cards(
	struct s_player_action *action
);

/**
 * @brief Mark action as ready for processing
 * 
 * Sets the ready flag, signaling to game logic that this action
 * is complete and should be processed.
 * 
 * @param action The action structure to modify
 * @return 0 on success, non-zero on error
 */
int						action_ready(
	struct s_player_action *action
);

/* ========================================================================== */
/*                       ACTION PROCESSING FUNCTION                           */
/* ========================================================================== */

// Forward declarations
struct s_game_manager;
struct s_game_local;

/**
 * @brief Handle and validate a player action
 * 
 * Processes a player action:
 * - Validates the action is legal according to game rules
 * - Updates game state (hands, pile, etc.)
 * - Checks for special card effects
 * - Advances turn or triggers phase transitions
 * 
 * This is called by the game logic loop when an action is ready.
 * 
 * @param manager The game manager
 * @param game The local game state
 * @param action The player action to process
 * @return 0 on success, non-zero on error
 */
int	_handle_player_action(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
);

#endif
