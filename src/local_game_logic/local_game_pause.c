
# include "game/game_data.h"
# include "game/game_local.h"

static int	_continue_game(
	struct s_menu *menu,
	struct notcurses *nc
);

static int	pause_input_handler(
	struct s_game_manager *manager,
	struct ncinput event
)
{
	struct s_game_local_pause	*main_menu;
	struct s_menu				*menu;

	if (!manager || !manager->state_data)
		return (1);
	main_menu = (struct s_game_local_pause *)manager->state_data;
	if (!main_menu)
		return (1);
	menu = main_menu->menu;
	if (event.id == NCKEY_DOWN)
		return (menu_select_next(menu));
	else if (event.id == NCKEY_UP)
		return (menu_select_prev(menu));
	else if (event.id == NCKEY_ENTER || event.id == '\n' || event.id == '\r')
		return (menu_activate_selected(menu));
	else if (event.id == 'p')
		_continue_game(menu, manager->nc);
	return (0);
}

static int	_continue_game(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	struct s_game_manager	*manager;

	if (!menu || !nc)
		return (1);
	manager = menu->user_data;
	if (!manager)
		return (1);
	manager->next_state = GAME_STATE_GAME_LOCAL_PLAY;
	return (0);
}

static int	_exit_game(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	struct s_game_manager	*manager;

	if (!menu || !nc)
		return (1);
	manager = menu->user_data;
	if (!manager)
		return (1);
	// Is it now my responsibility to free the game data?
	// I suppose it is right
	if (manager->prev_state == GAME_STATE_GAME_LOCAL_PLAY && manager->prev_state_data && manager->prev_state_data_destructor)
	{
		manager->prev_state_data_destructor(manager->prev_state_data);
		manager->prev_state_data = NULL;
		manager->prev_state_data_destructor = NULL;
	}
	manager->next_state = GAME_STATE_MAIN;
	return (0);
}

int		load_local_pause(struct s_game_manager *manager, void **state_data)
{
	struct s_game_local_pause	*pause_state;
	struct s_menu_option		options[] = {
		{.text_type = STATIC_TEXT, .option_text = "Continue", .option_action = _continue_game},
		{.text_type = STATIC_TEXT, .option_text = "Back to main menu", .option_action = _exit_game}
	};

	if (!manager || !state_data)
		return (1);
	pause_state = ft_calloc(1, sizeof(struct s_game_local_pause));
	if (!pause_state)
		return (MANAGER_RET_ERR("Unable to allocate pause state data"));
	if (menu_create(&pause_state->menu, notcurses_stdplane(manager->nc),
		options, sizeof(options) / sizeof(options[0])))
		return (free(pause_state), MANAGER_RET_ERR("Unable to create pause menu"));
	pause_state->menu->user_data = manager;
	manager->stdin_handler = pause_input_handler;
	manager->renderers[0].data = pause_state->menu;
	manager->renderers[0].render_fn = (t_renderer_fn)menu_render;
	manager->renderer_count = 1;
	manager->state_data_destructor = (t_freefn)free_local_pause;
	(*state_data) = pause_state;
	return (0);
}

void	unload_local_pause(struct s_game_manager *manager, void *state_data)
{
	if (!manager || !state_data)
		return ;
	free_local_pause(state_data);
	manager->stdin_handler = NULL;
	if (manager->prev_state == GAME_STATE_GAME_LOCAL_PLAY
		&& manager->prev_state_data)
	{
		manager->state_data = manager->prev_state_data;
		manager->state_data_destructor = manager->prev_state_data_destructor;
	}
	else
	{
		manager->state_data = NULL;
		manager->state_data_destructor = NULL;
	}
}

void	free_local_pause(struct s_game_local_pause *pause_state)
{
	if (!pause_state)
		return ;
	if (pause_state->menu)
		menu_destroy(pause_state->menu);
	free(pause_state);
}

