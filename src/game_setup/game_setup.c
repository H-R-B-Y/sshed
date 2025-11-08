
# include "game/game_data.h"

static int	setup_exit(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	struct s_game_manager	*manager;

	if (!menu || !nc)
		return (1);
	manager = (struct s_game_manager *)menu->user_data;
	if (!manager)
		return (1);
	manager->next_state = GAME_STATE_MAIN;
	return (0);
}

static int	setup_start_game(
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
	if (manager->state != GAME_STATE_GAME_LOCAL_SETUP)
		return (1);
	manager->next_state = GAME_STATE_GAME_LOCAL_PLAY;
	return (0);
}

static int	setup_input_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
)
{
	struct s_game_local_setup	*setup = (struct s_game_local_setup *)manager->state_data;
	struct s_menu				*menu = setup->menu;
	int							c_code;

	if (!manager || !manager->state_data || !event)
		return (1);
	if (event->data.fd != manager->reading_fd)
		return (1);
	c_code = notcurses_get_blocking(manager->nc, NULL);
	if (c_code == NCKEY_DOWN)
		return (menu_select_next(menu));
	else if (c_code == NCKEY_UP)
		return (menu_select_prev(menu));
	else if (c_code == NCKEY_ENTER || c_code == '\n' || c_code == '\r')
		return (menu_activate_selected(menu));
	return (0);
}

static char	*get_player_count_text(struct s_menu *menu, struct s_menu_option *option)
{
	struct s_game_manager		*manager;
	struct s_game_local_setup	*setup;
	char						*output;

	if (!menu)
		return (NULL);
	manager = menu->user_data;
	if (!manager)
		return (NULL);
	setup = manager->state_data;
	if (!setup || manager->state != GAME_STATE_GAME_LOCAL_SETUP)
		return (NULL);
	output = ft_calloc(12, sizeof(char)); // Players: 1 : 10 chars so we need 12? (for safety)
	if (sprintf(output, "Players: %u", setup->player_count) == 0)
		return (free(output), NULL);
	return (output);
}

static int	setup_players(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	struct s_game_manager		*manager;
	struct s_game_local_setup	*setup;

	(void)nc;
	if (!menu)
		return (1);
	manager = menu->user_data;
	if (!manager)
		return (1);
	setup = manager->state_data;
	if (!setup || manager->state != GAME_STATE_GAME_LOCAL_SETUP)
		return (1);
	setup->player_count++;
	if (setup->player_count == 4)
		setup->player_count = 1;
	setup->menu->is_dirty = 1;
	return (0); // 
}


int	load_game_setup_state(
	struct s_game_manager *manager,
	void **state_data
)
{
	struct s_game_local_setup		*setup;
	struct s_menu_option			options[] = {
		{.text_type = STATIC_TEXT, .option_text = "Start", .option_action = setup_start_game},
		{.text_type = DYNAMIC_TEXT_FUNCTION, .get_option_text = get_player_count_text, .option_action = setup_players},
		{.text_type = STATIC_TEXT, .option_text = "Rules", .option_action = NULL},
		{.text_type = STATIC_TEXT, .option_text = "Back", .option_action = setup_exit},
	};

	if (!manager || !state_data)
		return (1);
	load_free_prev(manager);
	setup = ft_calloc(1, sizeof(struct s_game_local_setup));
	if (!setup)
		return (1);
	if (menu_create(
		&setup->menu,
		notcurses_stdplane(manager->nc),
		options,
		sizeof(options) / sizeof(options[0])
		))
		return (MANAGER_RET_ERR("Failed to create game setup menu"));
	manager->renderers[0] = (struct s_renderer){
		.render_fn = (t_renderer_fn)menu_render,
		.data = setup->menu
	};
	setup->player_count = 1;
	manager->renderer_count = 1;
	manager->stdin_handler = setup_input_handler;
	setup->menu->user_data = manager; // if menu actions need access to the manager
	*state_data = setup;
	return (0);
}

void	unload_game_setup_state(
	struct s_game_manager *manager,
	void *state_data
)
{
	struct s_game_local_setup		*setup;
	struct s_game_local_settings	*settings;

	if (!manager || !state_data)
		return ;
	setup = (struct s_game_local_setup *)state_data;
	settings = ft_calloc(1, sizeof(struct s_game_local_settings));
	if (settings)
	{
		settings->player_count = setup->player_count;
		unload_set_prev_data(manager, settings, free);
	}
	else
		unload_unset_prev(manager);
	menu_destroy(setup->menu);
	free(setup);
	unload_unset_data(manager);
	unload_unset_stdinhandler(manager);
	return ;
}
