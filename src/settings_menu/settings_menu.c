
# include "game/game_data.h"

// item callbacks

static int	settings_back_handler(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	struct s_game_manager		*manager;
	// struct s_game_settings_menu	*menu;

	if (!menu || !nc)
		return (1);
	manager = menu->user_data;
	if (!manager)
		return (1);
	manager->next_state = GAME_STATE_MAIN;
	return (0);
}

// stdin callback

static int	menu_input_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
)
{
	struct s_main_menu	*main_menu;
	struct s_menu		*menu;
	int					c_code;

	if (!manager || !event)
		return (1);
	main_menu = manager->state_data;
	if (!main_menu)
		return (1);
	menu = main_menu->menu;
	if (!menu)
		return (1);
	if (event->data.fd != manager->reading_fd)
		return (0);
	c_code = notcurses_get_blocking(manager->nc, NULL);
	if (c_code == NCKEY_DOWN)
		return (menu_select_next(menu));
	else if (c_code == NCKEY_UP)
		return (menu_select_prev(menu));
	else if (c_code == NCKEY_ENTER || c_code == '\n' || c_code == '\r')
		return (menu_activate_selected(menu));
	return (0);
}

// load function

int	load_settings_menu(
	struct s_game_manager *manager,
	void **state_data
)
{
	struct s_game_settings_menu	*data;
	struct s_menu_option		options[] = {
		{.text_type = STATIC_TEXT, .option_text = "Back", .option_action = settings_back_handler},
	};

	if (!manager || !state_data)
		return (1);
	data = ft_calloc(1, sizeof(struct s_game_settings_menu));
	if (!data)
		return (1);
	if (menu_create(
		&data->menu, notcurses_stdplane(manager->nc),
		options, sizeof(options) / sizeof(options[0])
		) != 0)
	{
		manager->errmsg = "Unable to create settings menu";
		free(data);
		return (1);
	}
	manager->stdin_handler = menu_input_handler;
	manager->renderers[0] = (struct s_renderer){
		.render_fn = (t_renderer_fn)menu_render,
		.data = data->menu
	};
	manager->renderer_count = 1;
	data->menu->user_data = manager;
	(*state_data) = data;
	return (0);
}

// unload function

void	unload_settings_menu(
	struct s_game_manager *manager,
	void *state_data
)
{
	struct s_game_settings_menu	*menu;

	if (!manager || !state_data)
		return ;
	menu = state_data;
	if (menu->menu)
		menu_destroy(menu->menu);
	free(menu);
	manager->state_data = NULL;
	manager->state_data_destructor = NULL;
	manager->stdin_handler = NULL;
	return ;
}
