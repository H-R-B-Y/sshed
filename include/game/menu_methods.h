
#ifndef MENU_METHODS_H
# define MENU_METHODS_H

# include "menu.h"
# include <notcurses/notcurses.h>

struct s_game_manager;
struct epoll_event;

struct s_main_menu // nothing else needed for now
{
	struct s_menu			*menu;
};

int load_main_menu_state(
	struct s_game_manager *manager,
	void **state_data
);
void	unload_main_menu_state(
	struct s_game_manager *manager,
	void *state_data
);

// These should probably be moved somewhere else

int	load_init_state(
	struct s_game_manager *manager,
	void **state_data
);

void	unload_init_state(
	struct s_game_manager *manager,
	void *state_data
);

int	load_game_setup_state(
	struct s_game_manager *manager,
	void **state_data
);

void	unload_game_setup_state(
	struct s_game_manager *manager,
	void *state_data
);

int	load_settings_menu(
	struct s_game_manager *manager,
	void **state_data
);

void	unload_settings_menu(
	struct s_game_manager *manager,
	void *state_data
);

#endif
