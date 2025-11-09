
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
	struct ncinput event
)
{
	struct s_game_settings_menu	*settings;
	struct s_menu		*menu;

	if (!manager)
		return (1);
	settings = manager->state_data;
	if (!settings)
		return (1);
	menu = settings->menu;
	if (!menu)
		return (1);
	if (event.id == NCKEY_DOWN)
		return (menu_select_next(menu));
	else if (event.id == NCKEY_UP)
		return (menu_select_prev(menu));
	else if (event.id == NCKEY_ENTER || event.id == '\n' || event.id == '\r')
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
	load_free_prev(manager);
	data = ft_calloc(1, sizeof(struct s_game_settings_menu));
	if (!data)
		return (1);
	if (menu_create(
		&data->menu, notcurses_stdplane(manager->nc),
		options, sizeof(options) / sizeof(options[0])) != 0)
		return (free(data), MANAGER_RET_ERR("Unable to create settings menu"));
	data->menu->user_data = data;
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
	unload_unset_data(manager);
	unload_unset_stdinhandler(manager);
	return ;
}
