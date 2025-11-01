#ifndef GAME_MANAGER_H
# define GAME_MANAGER_H

# include "game_types.h"
# include "main_statemap.h"
# include <notcurses/notcurses.h>


struct s_renderer
{
	/// @brief Function pointer to the render function
	t_renderer_fn	render_fn;
	/// @brief Data pointer to be passed to the render function
	void			*data;
};

// Main game manager structure
struct s_game_manager
{
	/// @brief Notcurses context
	struct notcurses	*nc;

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

/**
 * @brief Initialize the game manager
 * @param manager Pointer to store the initialized game manager
 * @return 0 on success, 1 on error
 */
int		game_manager_init(
	struct s_game_manager **manager
);

/**
 * @brief Destroy the game manager and free resources
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
 * It will set the clock for the timer fd to the defined refresh rate
 * 
 * @param manager The game manager
 * @return 0 on normal exit, 1 on error
 */
int		game_manager_run(
	struct s_game_manager *manager
);

/**
 * @brief Stop the game manager's main loop
 * @param manager The game manager to stop
 */
void	game_manager_stop(
	struct s_game_manager *manager
);

/**
 * @brief Change the current game state to the next state
 * 
 * This function, in sequence, will:
 * - Call the unload callback for the current state, if set.
 * - Change the prev state to the current state.
 * - Change the current state to the next state.
 * - Call the load callback for the new current state, if set.
 * 
 * @param manager The game manager
 * @return 0 on success, 1 on error
 */
int		game_manager_change_state(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for unsetting the previous state data in an unload callback
 * @param manager The game manager
 */
void	unload_unset_prev(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for unsetting the stdin handler in an unload callback
 * @param manager The game manager
 */
void	unload_unset_stdinhandler(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for unsetting the renderers in an unload callback
 * @param manager The game manager
 */
void	unload_unset_renderers(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for setting the previous state data in an unload callback
 * @param manager The game manager
 * @param data The data to set as previous state data
 * @param free_data The function to free the previous state data
 */
void	unload_set_prev_data(
	struct s_game_manager *manager,
	void *data,
	t_freefn free_data
);

/**
 * @brief Utility function for freeing the previous state data in a load callback
 * @param manager The game manager
 */
void	load_free_prev(
	struct s_game_manager *manager
);

/**
 * @brief Utility function for unsetting the current state data in an unload callback
 * @param manager The game manager
 */
void	unload_unset_data(
	struct s_game_manager *manager
);

/**
 * @brief cleanup on unload
 * This will perform all the following:
 *  - Unsets current state data
 *  - Unsets prev state data
 *  - Unsets renderers
 *  - Unsets stdin handler
 * @param manager The game manager
 */
void	unload_clean_all(
	struct s_game_manager *manager
);

#endif
