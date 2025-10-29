
#ifndef GAME_DATA_H
# define GAME_DATA_H


# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 1
# endif
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE
# endif
# ifndef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 700
# endif

# include "main_statemap.h"
# include "deck.h"
# include "deck_display.h"
# include "hand.h"
# include "pdisplay.h"
# include "pile_display.h"
# include "sock_client_framework.h"
# include "menu_methods.h"
# include "game_local.h"

# include <sys/timerfd.h>


# define MANAGER_RET_ERR(str) ((manager->errmsg = str), 1)


typedef int		(*t_state_load_fn)(
	struct s_game_manager *manager,
	void **state_data
);

typedef void	(*t_state_unload_fn)(
	struct s_game_manager *manager,
	void *state_data
);

// Handlers for each state
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
	t_state_load_fn		load_fn;
	/*
	Unload function checklist:
	- Free any state data allocated in load function (or move it to the prev_state_data)
	- Unset manager->stdin_handler if it was set
	- Nullify the current state data if needed
	- Set or unset prev state data
	- Return nothing ?
	*/
	t_state_unload_fn	unload_fn;
};

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

typedef int	(*t_renderer_fn)(void *data);
struct s_renderer
{
	t_renderer_fn	render_fn;
	void			*data;
};

typedef int	(*t_event_handler_fn)(
	struct s_game_manager *manager,
	struct epoll_event *event
);

struct s_game_manager
{
	struct notcurses	*nc;

	t_game_state	next_state;
	t_game_state	state;
	t_game_state	prev_state;

	void			*state_data; // pointer to data relevant to the current state
	t_freefn		state_data_destructor; // function to free the state data
	void			*prev_state_data; // pointer to data relevant to the previous state
	t_freefn		prev_state_data_destructor; // function to free the previous state data

	t_event_handler_fn	stdin_handler; // function to handle stdin events

	char			*errmsg;

	int				epoll_fd;
	int				timer_fd; // will signal refreshes
	int				reading_fd;
	int				running;
	struct s_renderer	renderers[32]; // max 32 renderers
	size_t			renderer_count;
};

int		game_manager_init(struct s_game_manager **manager);
void	game_manager_destroy(struct s_game_manager *manager);

int		game_manager_change_state(
	struct s_game_manager *manager
);
// Runs the game manager loop, does not return until exit
int		game_manager_run(
	struct s_game_manager *manager
);

void	game_manager_stop(
	struct s_game_manager *manager
);

struct s_game_local_setup
{
	unsigned int			player_count;
	// Some struct to contain the game custom rules
	// The menu struct
	struct s_menu			*menu;
};

struct s_game_settings_menu
{
	struct s_menu			*menu;
};


// So our initial state is just the menu
// We initialize the menu with options to start game, settings, exit etc.
// Each option has a callback that modifies the game state, since we are running in a loop using epoll around notcurses_get_blocking
// We can just change the game state and let the main loop handle the rest.

#endif
