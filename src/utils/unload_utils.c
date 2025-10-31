
# include "game/game_data.h"


void	unload_unset_prev(
	struct s_game_manager *manager
)
{
	if (!manager)
		return ;
	manager->prev_state_data = NULL;
	manager->prev_state_data_destructor = NULL;
}

void	unload_unset_stdinhandler(
	struct s_game_manager *manager
)
{
	if (!manager)
		return ;
	manager->stdin_handler = NULL;
}

void	unload_set_prev_data(
	struct s_game_manager *manager,
	void *data,
	t_freefn free_data
)
{
	if (!manager)
		return ;
	manager->prev_state_data = data;
	manager->prev_state_data_destructor = free_data;
}

void	unload_unset_data(
	struct s_game_manager *manager
)
{
	if (!manager)
		return ;
	manager->state_data = NULL;
	manager->state_data_destructor = NULL;
}

void	unload_unset_renderers(
	struct s_game_manager *manager
)
{
	manager->renderer_count = 0;
}
void	unload_clean_all(
	struct s_game_manager *manager
)
{
	unload_unset_data(manager);
	unload_unset_prev(manager);
	unload_unset_renderers(manager);
	unload_unset_stdinhandler(manager);
}


void	load_free_prev(
	struct s_game_manager *manager
)
{
	if (!manager)
		return ;
	if (manager->prev_state_data)
	{
		if (manager->prev_state_data_destructor)
			manager->prev_state_data_destructor(manager->prev_state_data);
		else
			dprintf(STDERR_FILENO, "NO FREE FUNCTION IN STATE %s\n",
			game_state_str[manager->prev_state]);
		manager->prev_state_data = NULL;
		manager->prev_state_data_destructor = NULL;
	}
}


