/**
 * @file game_local.h
 * @brief Local game state structures and functions
 * 
 * This header defines structures and functions for local (single-player vs AI) gameplay.
 * It includes:
 * - Local game state machine (deal, swap, play, end phases)
 * - Local game state data structure
 * - Pause state structure
 * - End game state structure
 * - State handler functions (load/unload) for local game states
 * - Helper functions for game logic and visuals
 * 
 * The local game manages:
 * - Player and AI hands
 * - Deck and pile displays
 * - Turn management
 * - Game log
 * - Player actions and AI state machines
 */

#ifndef GAME_LOCAL_H
# define GAME_LOCAL_H

# include "main_statemap.h"
# include "menu.h"
# include "deck.h"
# include "deck_display.h"
# include "hand.h"
# include "pdisplay.h"
# include "pile_display.h"
# include "player_action.h"
# include "ai_state_machine.h"
# include "enum_macro.h"

/* ========================================================================== */
/*                              CONSTANTS                                     */
/* ========================================================================== */

/// @brief Timeout for game internal logic updates (frames)
# define FRAME_TIMEOUT 1

/// @brief Cards per player: 3 face down + 3 face up + 5 in hand
# define CARDS_PER_PLAYER 11

// Forward declarations
struct epoll_event;
struct s_game_manager;

/* ========================================================================== */
/*                         LOCAL GAME STATE PHASES                            */
/* ========================================================================== */

/**
 * @brief Play state phases within the local game
 * 
 * Defines the different phases of gameplay:
 * - NONE: No phase (initialization)
 * - DEAL_PHASE: Dealing cards to players
 * - SWAP_PHASE: Players swap cards between hand and shed
 * - PLAY_PHASE: Main gameplay phase
 * - GAME_END: Game has ended
 * - ERROR: Error state
 */
# define GAME_PLAYING_STATES(prefix, func) \
	func(prefix, NONE) \
	func(prefix, DEAL_PHASE) \
	func(prefix, SWAP_PHASE) \
	func(prefix, PLAY_PHASE) \
	func(prefix, GAME_END) \
	func(prefix, ERROR)

ENUM_FACTORY(PLAY_STATE, game_play_state, GAME_PLAYING_STATES);

/* ========================================================================== */
/*                         UI SELECTION ENUMS                                 */
/* ========================================================================== */

/**
 * @brief UI elements that can be selected during gameplay
 * 
 * Used for keyboard navigation between different UI components.
 */
enum e_selected_item
{
	SELECTED_ITEM_NONE,
	SELECTED_ITEM_HAND,
	SELECTED_ITEM_SWAP_MENU,
	SELECTED_ITEM_SWAP_PILE,
	SELECTED_ITEM_SWAP_HAND,
	SELECTED_ITEM_LOG,
	SELECTED_ITEM_ACTION_MENU,
	SELECTED_ITEM_COUNT
};

/* ========================================================================== */
/*                         STATE DATA STRUCTURES                              */
/* ========================================================================== */

/**
 * @brief Local game settings
 * 
 * Configuration for the local game session.
 */
struct s_game_local_settings
{
	/// @brief Number of players in the game (1 player + 0-3 AI)
	t_u8		player_count;
	/// @brief House rules and custom game settings
	/// @todo Expand this to include house rules like special card effects
};

/**
 * @brief Main local game state data structure
 * 
 * Contains all data needed for the local game playing state,
 * including game components, visual displays, and game logic state.
 */
struct s_game_local
{
	/// @brief Whether the game has been initialised
	t_u8							initialised;
	/// @brief Game settings
	struct s_game_local_settings	settings;
	/// @brief Currently selected UI element for keyboard navigation
	enum e_selected_item			selected_item;

	/* Game components */

	/// @brief The deck used in the game
	struct s_deck					*deck;
	/// @brief The deck display
	struct s_deck_display			*deck_display;
	/// @brief The player's hand
	struct s_hand					*hand;
	/// @brief The hands for each AI player
	struct s_pdisplay				*pdisplay[3];
	/// @brief Number of AI players in the game
	t_u8							pdisplay_count;
	/// @brief AI state machine data for each AI player
	struct s_ai_data				ai_data[3];
	/// @brief The pile display (play pile)
	struct s_pile_display			*pile_display;

	/* Game state */
	/// @brief Current play state (deal, swap, play, end)
	t_game_play_state				play_state;
	/// @brief Number of cards played in total this game
	/// @todo Move to stats structure
	int								cards_played;
	/// @brief Whose turn it is (0 = player, 1-3 = AI)
	t_u8							whos_turn;
	/// @brief The action struct representing the current player's action
	struct s_player_action			player_action;

	/* UI elements */

	/// @brief Menu for displaying player action options:
	/// - Default (pick up pile)
	/// - Play 1 card
	/// - Play 2 cards (if multiple cards of same rank)
	/// - Play 3 cards (if multiple cards of same rank)
	/// - Play 4 cards (if multiple cards of same rank)
	struct s_menu					*player_action_menu;

	/* Swap phase specific */

	/// @brief The swap pile (only exists during swap phase)
	struct s_pile_display			*swap_pile;
	/// @brief Menu to swap or not during the swap phase
	struct s_menu					*swap_menu;

	/* Game log */

	/// @brief Linked list of log lines
	/// @todo Define log entry structure properly
	t_list							*game_log;
	/// @brief Visual log plane for rendering
	struct ncplane					*log_plane;
	/// @brief Maximum number of log lines to keep
	unsigned int					max_log_line;
	/// @brief Whether the log is visible
	t_u8							log_visible;
	/// @brief Whether the log needs to be re-rendered
	t_u8							log_dirty;

	struct ncplane					*help_menu;

	/* End game data */

	/// @brief Who won the game (0 = player, 1-3 = AI)
	/// @note Should be passed to end game state
	/// @todo Better mechanism for passing end game data
	t_u8							who_won;
};

/**
 * @brief Local game pause state data structure
 * 
 * Contains data for the pause menu during local gameplay.
 */
struct s_game_local_pause
{
	/// @brief The pause menu
	struct s_menu		*menu;
};

/**
 * @brief Local game end state data structure
 * 
 * Contains data for the end game screen showing results and log.
 */
struct s_game_local_end
{
	/// @brief Who won the game (0 = player, 1-3 = AI)
	t_u8					who_won;
	/// @brief Linked list of log lines from the game
	t_list					*log_data;
	/// @brief Visual log plane for rendering
	struct ncplane			*log_plane;
	/// @brief Whether the log is visible
	t_u8					log_visible;
	/// @brief Whether the log needs to be re-rendered
	t_u8					log_dirty;
	/// @brief The end game menu (return to main menu, etc.)
	struct s_menu			*menu;
};

/* ========================================================================== */
/*                    LOCAL GAME STATE HANDLER FUNCTIONS                      */
/* ========================================================================== */

/**
 * @brief Load function for the local game state
 * 
 * This function checks the previous state:
 * - If coming from paused state: restore that state
 * - If coming from setup state: load settings from setup
 * - Otherwise: load default settings
 * 
 * Initializes all game components (deck, hands, displays, etc.)
 * 
 * @param manager The game manager
 * @param state_data Pointer to store the loaded state data
 * @return 0 on success, 1 on error
 */
int		load_game_local(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload function for the local game state
 * 
 * Handles cleanup when leaving the game state.
 * May preserve state to prev_state_data if pausing.
 * 
 * @param manager The game manager
 * @param state_data The state data to unload
 */
void	unload_game_local(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Free function for the local game state
 * 
 * Frees all allocated resources for the game state including:
 * - Deck and displays
 * - Hands and player displays
 * - Menus
 * - Game log
 * 
 * @param game The game local state to free
 */
void	free_game_state(
	struct s_game_local *game
);

/* ========================================================================== */
/*                    PAUSE STATE HANDLER FUNCTIONS                           */
/* ========================================================================== */

/**
 * @brief Load function for the local game pause state
 * 
 * Creates the pause menu with options to resume or quit.
 * 
 * @param manager The game manager
 * @param state_data Pointer to store the loaded state data
 * @return 0 on success, 1 on error
 */
int		load_local_pause(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload function for the local game pause state
 * 
 * Cleans up the pause menu.
 * 
 * @param manager The game manager
 * @param state_data The state data to unload
 */
void	unload_local_pause(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Free function for the local game pause state
 * 
 * Frees the pause menu and associated resources.
 * 
 * @param pause The pause state to free
 */
void	free_local_pause(
	struct s_game_local_pause *pause
);

/* ========================================================================== */
/*                    END GAME STATE HANDLER FUNCTIONS                        */
/* ========================================================================== */

/**
 * @brief Load function for the local game end state
 * 
 * Retrieves winner information from previous state data.
 * Creates the end game menu and displays results.
 * If no previous state data available, defaults to player winning.
 * 
 * @param manager The game manager
 * @param state_data Pointer to store the loaded state data
 * @return 0 on success, 1 on error
 */
int		load_local_end(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload function for the local game end state
 * 
 * Cleans up the end game screen.
 * 
 * @param manager The game manager
 * @param state_data The state data to unload
 */
void	unload_local_end(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Free function for the local game end state
 * 
 * Frees the end game menu, log data, and associated resources.
 * 
 * @param end_state The end state to free
 */
void	free_local_end(
	struct s_game_local_end *end_state
);

/* ========================================================================== */
/*                       GAME LOGIC FUNCTIONS                                 */
/* ========================================================================== */

/**
 * @brief Render loop callback to update the game logic
 * 
 * This function handles game state updates each frame:
 * - Processes player actions
 * - Updates AI state machines
 * - Checks win conditions
 * - Handles card dealing
 * - Manages phase transitions (deal -> swap -> play -> end)
 * 
 * @param manager The game manager
 * @return 0 on success, 1 on error
 */
int		pre_render_game_update(
	struct s_game_manager *manager
);

/**
 * @brief Input handler for the local game play state
 * 
 * Processes player input events during gameplay:
 * - Card selection
 * - Menu navigation
 * - Action confirmation
 * - Pause activation
 * 
 * @todo Update to handle new UI selection model with multiple selectable elements
 * 
 * @param manager The game manager
 * @param event The epoll event containing input data
 * @return 0 on success, 1 on error
 */
int		game_local_play_input_handler(
	struct s_game_manager *manager,
	struct ncinput event
);

/* ========================================================================== */
/*                       VISUAL HELPER FUNCTIONS                              */
/* ========================================================================== */

/**
 * @brief Reorder visual elements (z-order/layering)
 * 
 * Ensures proper stacking order of visual elements for rendering.
 * Called when visual hierarchy needs to be updated.
 * 
 * @param manager The game manager
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int		re_order_visuals(
	struct s_game_manager *manager,
	struct s_game_local *game
);

/* ========================================================================== */
/*                       SWAP PHASE HELPER FUNCTIONS                          */
/* ========================================================================== */

/**
 * @brief Clear selection in the swap pile
 * 
 * Deselects any selected cards in the swap pile display.
 * 
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int		swap_pile_clear_selection(
	struct s_game_local *game
);

/**
 * @brief Select the swap menu for keyboard navigation
 * 
 * Changes the selected UI element to the swap menu.
 * 
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int		game_local_select_menu(
	struct s_game_local *game
);

/**
 * @brief Select the swap pile for keyboard navigation
 * 
 * Changes the selected UI element to the swap pile.
 * 
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int		game_local_select_pile(
	struct s_game_local *game
);

/**
 * @brief Select the hand for keyboard navigation during swap phase
 * 
 * Changes the selected UI element to the player's hand.
 * 
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int		game_local_select_swap_hand(
	struct s_game_local *game
);

/**
 * @brief Deselect all UI elements
 * 
 * @param game The game local state
 * @return int 
 */
int	game_local_select_nothing(struct s_game_local *game);

/**
 * @brief Increment whose turn it is
 * 
 * @param game The game local state
 * @return int 
 */
int game_local_increment_whos_turn(struct s_game_local *game);

/**
 * @brief Determine who won the game
 * 
 * @param game The game local state
 * @return int 
 */
int game_local_who_won(struct s_game_local *game);

/**
 * @brief Unload the swap phase data and visuals
 * @warning This is not a state unload function it just cleans up the swap phase visuals
 * @param manager The game manager
 * @param game The game local state
 * @return int 
 */
int unload_swap_phase(struct s_game_manager *manager, struct s_game_local *game);

/**
 * @brief Select the action menu for keyboard navigation
 * 
 * Changes the selected UI element to the player action menu.
 * 
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int game_local_select_action_menu(struct s_game_local *game);

/**
 * @brief Initialize the action menu
 * 
 * Creates and sets up the player action menu.
 * 
 * @param manager The game manager
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int game_local_action_menu_init(
	struct s_game_manager *manager,
	struct s_game_local *game
);

/**
 * @brief Cleanup the action menu
 * 
 * Hides and frees the player action menu.
 * 
 * @param manager The game manager
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int game_local_action_menu_cleanup(
	struct s_game_manager *manager,
	struct s_game_local *game
);


/**
 * @brief Select the hand for keyboard navigation
 * 
 * Changes the selected UI element to the player's hand.
 * 
 * @param game The game local state
 * @return 0 on success, 1 on error
 */
int game_local_select_hand(struct s_game_local *game);

/**
 * @brief Setup the render hooks for the swap phase
 * 
 * @param manager The game manager
 * @param game The game local state
 * @return int 
 */
int	swap_phase_render_hooks(struct s_game_manager *manager, struct s_game_local *game);

/**
 * @brief Setup the render hooks for the action menu
 * 
 * @param manager The game manager
 * @param local The game local state
 * @return int 
 */
int	game_local_action_menu_render_hooks(
	struct s_game_manager *manager,
	struct s_game_local *game
);
/**
 * @brief Reset phase data
 * 
 * @return int 
 */
int	reset_phase_data(void);

/**
 * @brief Initialise the swap phase data
 * 
 * @param manager The game manager
 * @param game The game state data
 * @return int 
 */
int	init_swap_phase(struct s_game_manager *manager, struct s_game_local *game);

#endif
