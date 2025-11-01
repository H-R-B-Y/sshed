#ifndef GAME_TYPES_H
# define GAME_TYPES_H

# include <sys/epoll.h>
# include "libft.h"

// Forward declarations
struct s_game_manager;

// Constants
# define MAX_RENDERERS 32
# define REFRESH_RATE_NS 16666666  // ~60Hz (you had an extra digit)
# define MAX_PLAYERS 4

// Error handling macro
# define MANAGER_RET_ERR(str) ((manager->errmsg = str), 1)

/**
 * @brief Load state function pointer type
 * Load function signature.
 * 
 * A load function should take care of the following:
 * - Allocate and initialize state data
 * - Set manager->state_data to the allocated data
 * - Set manager->state_data_destructor if needed
 * - Set manager->stdin_handler if needed
 * - Return 0 on success, non-zero on failure
 * 
 * @defgroup t_state_load_fn State Load Function Pointer Type
 */
typedef int		(*t_state_load_fn)(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload state function pointer type
 * Unload function signature.
 *
 * A unload function should take care of the following:
 * - Free or move current state to prev
 * - Clear current data
 * - Clear prev data (if applicable)
 * - Unset renderers
 * - Unset stdin handler
 * 
 * @defgroup t_state_unload_fn State Unload Function Pointer Type
 */
typedef void	(*t_state_unload_fn)(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Render function pointer type
 * Render function signature.
 * 
 * A render function should take care of rendering the provided data.
 * It should return 0 on success, non-zero on failure.
 * 
 * @defgroup t_renderer_fn Renderer Function Pointer Type
 */
typedef int		(*t_renderer_fn)(void *data);

/**
 * @brief Event handler function pointer type
 * Event handler function signature.
 * 
 * An event handler function should process the provided event.
 * It should return 0 on success, non-zero on failure.
 */
typedef int		(*t_event_handler_fn)(
	struct s_game_manager *manager,
	struct epoll_event *event
);

#endif
