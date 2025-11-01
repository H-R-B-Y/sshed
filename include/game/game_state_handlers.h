#ifndef GAME_STATE_HANDLERS_H
# define GAME_STATE_HANDLERS_H

# include "game_types.h"
# include "main_statemap.h"

/**
 * @brief Load the init state for the program
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

/**
 * @brief Load the main menu state
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

/**
 * @brief Load the settings menu state
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

/**
 * @brief Load the game setup state
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
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_game_setup_state(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Load the local game state
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
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_game_local(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Load the local game pause state
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
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_local_pause(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Load the local game end state
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
 * @ingroup t_state_unload_fn
 * @param manager Pointer to the game manager
 * @param state_data Pointer to the state data to unload
 */
void	unload_local_end(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief State handler structure containing load and unload function pointers
 * 
 * This structure pairs a load function with an unload function for each game state.
 * The load function is responsible for setting up the state, and the unload function
 * is responsible for cleaning up when transitioning away from the state.
 */
struct s_state_handler
{
	/*
	Load function checklist:
	- Allocate and initialize state data
	- Set manager->state_data to the allocated data
	- Set manager->state_data_destructor if needed
	- Set manager->stdin_handler if needed
	- Return 0 on success, non-zero on failure
	*/
	/// @brief Function to load/initialize the state
	t_state_load_fn		load_fn;
	/*
	Unload function checklist:
	- Free or move current state to prev
	- Clear current data
	- Clear prev data (if applicable)
	- Unset renderers
	- Unset stdin handler
	*/
	/// @brief Function to unload/cleanup the state
	t_state_unload_fn	unload_fn;
};

/**
 * @brief State handlers lookup table
 * 
 * This array maps each game state enum value to its corresponding load and unload functions.
 * Used by the game manager to determine which functions to call during state transitions.
 * 
 * @note The __attribute__((unused)) prevents compiler warnings about the static array
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
