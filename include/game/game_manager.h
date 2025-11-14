/**
 * @file game_manager.h
 * @brief Core game manager structure and interface
 * 
 * This header defines the main game manager structure that controls the
 * overall game loop, state management, rendering, and event handling.
 * 
 * The game manager:
 * - Manages state transitions
 * - Coordinates rendering through multiple renderers
 * - Handles input events via stdin handler
 * - Maintains timing through epoll and timer fd
 * - Provides utility functions for state load/unload operations
 */

#ifndef GAME_MANAGER_H
# define GAME_MANAGER_H

# include "game_types.h"
# include "main_statemap.h"
# include <notcurses/notcurses.h>

/* ========================================================================== */
/*                         CORE DATA STRUCTURES                               */
/* ========================================================================== */

/**
 * @brief Renderer structure
 * 
 * Pairs a render function with its associated data.
 * Multiple renderers can be registered per state.
 */
struct s_renderer
{
	/// @brief Function pointer to the render function
	t_renderer_fn	render_fn;
	/// @brief Data pointer to be passed to the render function
	void			*data;
};

/**
 * @brief Settings for the client
 * 
 * Should be accessed through the manager.
 * Items should be loaded from the environment.
 */
struct s_game_settings
{
	unsigned int	refresh_timeout;
	t_u8			auto_sort;
	unsigned int	ai_timeout;
	t_u32			background_channel;
	char			*display_name;
	t_u8			key_mode;
};

/**
 * @brief Set the up settings object from environment variables
 * 
 * @param manager The manager to set the settings for
 * @return int 
 */
int	setup_settings(struct s_game_manager *manager);

/**
 * @brief Main game manager structure
 * 
 * Central control structure for the entire game engine.
 * Manages state, rendering, events, and the main game loop.
 */
struct s_game_manager
{
	/// @brief Notcurses context
	struct notcurses	*nc;

	struct s_game_settings	settings;

	/// @brief Next game state
	/// @note If this is not GAME_STATE_NONE and not the same as current state,
	/// the manager will attempt to transition to this state in the next iteration of the mainloop.
	t_game_state		next_state;
	/// @brief Current game state
	t_game_state		state;
	/// @brief Previous game state
	t_game_state		prev_state;

	/// @brief The data for the current state
	void				*state_data;
	/// @brief Function to free the current state data
	t_freefn			state_data_destructor;
	/// @brief The data for the previous state
	void				*prev_state_data;
	/// @brief Function to free the previous state data
	t_freefn			prev_state_data_destructor;

	/// @brief Function pointer for handling stdin events for the current state
	t_event_handler_fn	stdin_handler; // function to handle stdin events

	/// @brief Error message string, if populated we encountered an error
	char				*errmsg;

	/// @brief Epoll file descriptor for event handling
	int					epoll_fd;
	/// @brief Timer file descriptor triggers each main loop iteration
	int					timer_fd; 
	/// @brief File descriptor for reading stdin events provided by notcurses
	int					reading_fd;
	/// @brief Running flag for the main loop
	int					running;

	/// @brief Array of renderers to be called each frame for the current state
	struct s_renderer	renderers[MAX_RENDERERS];
	/// @brief Number of active renderers in the renderers array
	size_t				renderer_count;
};

/* ========================================================================== */
/*                    GAME MANAGER LIFECYCLE FUNCTIONS                        */
/* ========================================================================== */

/**
 * @brief Initialize the game manager
 * 
 * Allocates and initializes the game manager structure, setting up:
 * - Notcurses context
 * - Epoll file descriptor
 * - Timer file descriptor
 * - Initial state to NONE
 * 
 * @param manager Pointer to store the initialized game manager
 * @return 0 on success, 1 on error
 */
int		game_manager_init(
	struct s_game_manager **manager
);

/**
 * @brief Destroy the game manager and free resources
 * 
 * Cleans up the game manager by:
 * - Closing file descriptors (epoll, timer, reading)
 * - Destroying the notcurses context
 * - Freeing state data
 * - Freeing the manager structure itself
 * 
 * @param manager The game manager to destroy
 * @note This function is compatible with t_freefn for use as a destructor callback
 */
void	game_manager_destroy(
	struct s_game_manager *manager
);

/**
 * @brief Runs the game manager loop, does not return until exit
 * 
 * This function starts the main loop of the game manager.
 * It will:
 * - Set the timer fd to the defined refresh rate
 * - Process events (timer, stdin) via epoll
 * - Call renderers each frame
 * - Handle state transitions
 * 
 * @param manager The game manager
 * @return 0 on normal exit, 1 on error
 */
int		game_manager_run(
	struct s_game_manager *manager
);

/**
 * @brief Stop the game manager's main loop
 * 
 * Sets the running flag to 0, causing the main loop to exit.
 * 
 * @param manager The game manager to stop
 */
void	game_manager_stop(
	struct s_game_manager *manager
);

/* ========================================================================== */
/*                       STATE TRANSITION FUNCTIONS                           */
/* ========================================================================== */

/**
 * @brief Change the current game state to the next state
 * 
 * This function performs state transition in sequence:
 * 1. Call the unload callback for the current state (if set)
 * 2. Set prev_state to current state
 * 3. Set current state to next_state
 * 4. Call the load callback for the new current state (if set)
 * 
 * @param manager The game manager
 * @return 0 on success, 1 on error
 */
int		game_manager_change_state(
	struct s_game_manager *manager
);

/* ========================================================================== */
/*                      UNLOAD UTILITY FUNCTIONS                              */
/* ========================================================================== */

/**
 * @brief Utility function for unsetting the previous state data in an unload callback
 * 
 * Frees prev_state_data using the prev_state_data_destructor and sets both to NULL.
 * 
 * @param manager The game manager
 */
void	unload_unset_prev(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for unsetting the stdin handler in an unload callback
 * 
 * Sets stdin_handler to NULL.
 * 
 * @param manager The game manager
 */
void	unload_unset_stdinhandler(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for unsetting the renderers in an unload callback
 * 
 * Clears all renderers and sets renderer_count to 0.
 * 
 * @param manager The game manager
 */
void	unload_unset_renderers(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for setting the previous state data in an unload callback
 * 
 * Transfers current state data to previous state data for later retrieval.
 * Useful when pausing a game state to resume later.
 * 
 * @param manager The game manager
 * @param data The data to set as previous state data
 * @param free_data The function to free the previous state data
 */
void	unload_set_prev_data(
	struct s_game_manager *manager,
	void *data,
	t_freefn free_data
);

/* ========================================================================== */
/*                       LOAD UTILITY FUNCTIONS                               */
/* ========================================================================== */

/**
 * @brief Utility function for freeing the previous state data in a load callback
 * 
 * Frees prev_state_data using the prev_state_data_destructor and sets both to NULL.
 * Useful when transitioning to a new state that doesn't need the previous data.
 * 
 * @param manager The game manager
 */
void	load_free_prev(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for unsetting the current state data in an unload callback
 * 
 * Sets state_data and state_data_destructor to NULL without freeing.
 * Use when the data has been transferred elsewhere (e.g., to prev_state_data).
 * 
 * @param manager The game manager
 */
void	unload_unset_data(
	struct s_game_manager *manager
);

/**
 * @brief Cleanup on unload - comprehensive state cleanup
 * 
 * This performs all standard cleanup operations:
 * - Unsets current state data
 * - Unsets prev state data
 * - Unsets renderers
 * - Unsets stdin handler
 * 
 * Use this for simple states that don't need to preserve any data.
 * 
 * @param manager The game manager
 */
void	unload_clean_all(
	struct s_game_manager *manager
);

#endif
