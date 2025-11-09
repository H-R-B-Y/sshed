/**
 * @file game_types.h
 * @brief Core type definitions and function pointers for the game state system
 * 
 * This header defines fundamental types used throughout the game engine:
 * - Function pointer types for state management (load, unload, render)
 * - Event handler function types
 * - Core constants (max players, refresh rate, etc.)
 * - Error handling macros
 * 
 * This is a foundational header that should be included by other game headers
 * that need these type definitions.
 */

#ifndef GAME_TYPES_H
# define GAME_TYPES_H

# include <sys/epoll.h>
# include "libft.h"

// Forward declarations
struct s_game_manager;

/* ========================================================================== */
/*                              CONSTANTS                                     */
/* ========================================================================== */

/// @brief Maximum number of concurrent renderers per game state
# define MAX_RENDERERS 32

/// @brief Refresh rate in nanoseconds (~60Hz)
# define REFRESH_RATE_NS 16666666

/// @brief Maximum number of players supported in a game
# define MAX_PLAYERS 4

/* ========================================================================== */
/*                           ERROR HANDLING                                   */
/* ========================================================================== */

/**
 * @brief Convenience macro for returning an error from game manager functions
 * @param str Error message string to set
 * @return Always returns 1 (error code)
 */
# define MANAGER_RET_ERR(str) ((manager->errmsg = str), 1)

/* ========================================================================== */
/*                        FUNCTION POINTER TYPES                              */
/* ========================================================================== */

/**
 * @brief Load state function pointer type
 * 
 * Load function signature for initializing a game state.
 * 
 * A load function should perform the following tasks:
 * - Allocate and initialize state-specific data
 * - Set manager->state_data to point to the allocated data
 * - Set manager->state_data_destructor if cleanup is needed
 * - Set manager->stdin_handler if the state handles input
 * - Configure any renderers needed for visual updates
 * 
 * @param manager The game manager context
 * @param state_data Pointer to store the allocated state data
 * @return 0 on success, non-zero on failure
 * 
 * @defgroup t_state_load_fn State Load Function Pointer Type
 */
typedef int		(*t_state_load_fn)(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload state function pointer type
 * 
 * Unload function signature for cleaning up a game state.
 *
 * An unload function should perform the following tasks:
 * - Free or preserve current state data to prev_state_data
 * - Clear current state data pointers
 * - Clear previous state data if applicable
 * - Unset all renderers
 * - Unset stdin handler
 * 
 * @param manager The game manager context
 * @param state_data The state data to unload/cleanup
 * 
 * @defgroup t_state_unload_fn State Unload Function Pointer Type
 */
typedef void	(*t_state_unload_fn)(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Render function pointer type
 * 
 * Render function signature for drawing visual elements.
 * 
 * A render function should handle rendering the provided data to the screen.
 * Multiple renderers can be registered per state and are called each frame.
 * 
 * @param data Context-specific data to render (cast to appropriate type)
 * @return 0 on success, non-zero on failure
 * 
 * @defgroup t_renderer_fn Renderer Function Pointer Type
 */
typedef int		(*t_renderer_fn)(void *data);

/**
 * @brief Event handler function pointer type
 * 
 * Event handler function signature for processing system events.
 * 
 * An event handler processes events from the epoll event loop, typically
 * stdin/keyboard input or timer events for the current game state.
 * 
 * @param manager The game manager context
 * @param event The epoll event to process
 * @return 0 on success, non-zero on failure
 */
typedef int		(*t_event_handler_fn)(
	struct s_game_manager *manager,
	struct ncinput event
);
#endif
