/**
 * @file game_state_handlers.h
 * @brief State handler function declarations and lookup table
 * 
 * This header is the central registry for all game state load/unload functions.
 * It provides:
 * - Function declarations for all state handlers
 * - State handler structure pairing load/unload functions
 * - Lookup table mapping game states to their handlers
 * 
 * This is the SINGLE SOURCE OF TRUTH for state handler function declarations.
 * Other headers should NOT duplicate these declarations.
 * 
 * Each game state defined in main_statemap.h should have corresponding
 * load and unload functions declared here.
 */

#ifndef GAME_STATE_HANDLERS_H
# define GAME_STATE_HANDLERS_H

# include "game_types.h"
# include "main_statemap.h"

/* ========================================================================== */
/*                    STATE HANDLER FUNCTION DECLARATIONS                     */
/* ========================================================================== */

/* -------------------------------------------------------------------------- */
/*                             Init State                                     */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load the init state for the program
 * 
 * Initialization state that runs once at program start.
 * Sets up initial resources and transitions to main menu.
 * 
 * @ingroup t_state_load_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, 1 on error
 */
int		load_init_state(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload the init state for the program
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_init_state(
	struct s_game_manager *manager,
	void *state_data
);

/* -------------------------------------------------------------------------- */
/*                           Main Menu State                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load the main menu state
 * 
 * Creates the main menu with options to start game, settings, or quit.
 * 
 * @ingroup t_state_load_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, 1 on error
 */
int		load_main_menu_state(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload the main menu state
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_main_menu_state(
	struct s_game_manager *manager,
	void *state_data
);

/* -------------------------------------------------------------------------- */
/*                          Settings Menu State                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load the settings menu state
 * 
 * Creates the settings menu for configuring game options.
 * 
 * @ingroup t_state_load_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, 1 on error
 */
int		load_settings_menu(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload the settings menu state
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_settings_menu(
	struct s_game_manager *manager,
	void *state_data
);

/* -------------------------------------------------------------------------- */
/*                         Game Setup State                                   */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load the game setup state
 * 
 * Creates the game setup menu for configuring local game options
 * (player count, house rules, etc.).
 * 
 * @ingroup t_state_load_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, 1 on error
 */
int		load_game_setup_state(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload the game setup state
 * 
 * Saves setup configuration to be used by the local game state.
 * 
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_game_setup_state(
	struct s_game_manager *manager,
	void *state_data
);

/* -------------------------------------------------------------------------- */
/*                        Local Game Play State                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load the local game state
 * 
 * Initializes the local game (player vs AI) from setup or paused state.
 * See game_local.h for full documentation.
 * 
 * @ingroup t_state_load_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, 1 on error
 */
int		load_game_local(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload the local game state
 * 
 * Cleans up or preserves the local game state.
 * See game_local.h for full documentation.
 * 
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_game_local(
	struct s_game_manager *manager,
	void *state_data
);

/* -------------------------------------------------------------------------- */
/*                       Local Game Pause State                               */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load the local game pause state
 * 
 * Creates the pause menu for a paused local game.
 * See game_local.h for full documentation.
 * 
 * @ingroup t_state_load_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, 1 on error
 */
int		load_local_pause(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload the local game pause state
 * 
 * Cleans up the pause menu.
 * See game_local.h for full documentation.
 * 
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_local_pause(
	struct s_game_manager *manager,
	void *state_data
);

/* -------------------------------------------------------------------------- */
/*                        Local Game End State                                */
/* -------------------------------------------------------------------------- */

/**
 * @brief Load the local game end state
 * 
 * Creates the end game screen showing results.
 * See game_local.h for full documentation.
 * 
 * @ingroup t_state_load_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, 1 on error
 */
int		load_local_end(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload the local game end state
 * 
 * Cleans up the end game screen.
 * See game_local.h for full documentation.
 * 
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_local_end(
	struct s_game_manager *manager,
	void *state_data
);

/* ========================================================================== */
/*                    STATE HANDLER LOOKUP STRUCTURE                          */
/* ========================================================================== */

/**
 * @brief State handler structure containing load and unload function pointers
 * 
 * This structure pairs a load function with an unload function for each game state.
 * The load function sets up the state, and the unload function cleans up when
 * transitioning away from the state.
 * 
 * Load function responsibilities:
 * - Allocate and initialize state data
 * - Set manager->state_data to the allocated data
 * - Set manager->state_data_destructor if needed
 * - Set manager->stdin_handler if needed
 * - Configure renderers
 * - Return 0 on success, non-zero on failure
 * 
 * Unload function responsibilities:
 * - Free or move current state to prev_state_data
 * - Clear current data pointers
 * - Clear prev data (if applicable)
 * - Unset renderers
 * - Unset stdin handler
 */
struct s_state_handler
{
	/// @brief Function to load/initialize the state
	t_state_load_fn		load_fn;
	/// @brief Function to unload/cleanup the state
	t_state_unload_fn	unload_fn;
};

/**
 * @brief State handlers lookup table
 * 
 * This array maps each game state enum value to its corresponding
 * load and unload functions. Used by the game manager to determine
 * which functions to call during state transitions.
 * 
 * The array is indexed by t_game_state values, so each state
 * automatically maps to the correct handlers.
 * 
 * @note The __attribute__((unused)) prevents compiler warnings
 * about the static array in header files.
 */
static const struct s_state_handler	state_handlers[GAME_STATE_COUNT]
__attribute__((unused)) = {
	[GAME_STATE_NONE]
		= { NULL, NULL },
	[GAME_STATE_INIT]
		= { load_init_state, unload_init_state },
	[GAME_STATE_MAIN]
		= { load_main_menu_state, unload_main_menu_state },
	[GAME_STATE_SETTINGS]
		= { load_settings_menu, unload_settings_menu},
	[GAME_STATE_GAME_LOCAL_SETUP]
		= { load_game_setup_state, unload_game_setup_state },
	[GAME_STATE_GAME_LOCAL_PLAY]
		= { load_game_local, unload_game_local },
	[GAME_STATE_GAME_LOCAL_PAUSE]
		= { load_local_pause, unload_local_pause },
	[GAME_STATE_GAME_LOCAL_END]
		= { load_local_end, unload_local_end },
	[GAME_STATE_GAME_NEW_STATE]
		= { NULL, NULL }
};

#endif
