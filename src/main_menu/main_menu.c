
#include "game/game_data.h"

static int	menu_go_to_local_setup(
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
	manager->next_state = GAME_STATE_GAME_LOCAL_SETUP;
	return (0);
}

static int	menu_exit_game(
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
	game_manager_stop(manager);
	return (0);
}

static int	menu_settings(
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
	manager->next_state = GAME_STATE_SETTINGS;
	return (0);
}

static int	menu_input_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
)
{
	struct s_main_menu	*main_menu;
	struct s_menu		*menu;

	if (!manager || !manager->state_data || !event)
		return (1);
	main_menu = (struct s_main_menu *)manager->state_data;
	if (!main_menu)
		return (1);
	menu = main_menu->menu;
	if (event->data.fd != manager->reading_fd)
		return (0);
	return (_menu_key_handler(menu, manager->nc));
}

int load_main_menu_state(
	struct s_game_manager *manager,
	void **state_data
)
{
	struct s_main_menu		*main_menu;
	struct s_menu_option	options[] = {
		{.text_type = STATIC_TEXT, .option_text = "Local Game", .option_action = menu_go_to_local_setup},
		{.text_type = STATIC_TEXT, .option_text = "Settings", .option_action = menu_settings},
		{.text_type = STATIC_TEXT, .option_text = "Exit", .option_action = menu_exit_game}
	};

	if (!manager || !state_data)
		return (1);
	load_free_prev(manager);
	notcurses_refresh(manager->nc, NULL, NULL);
	main_menu = ft_calloc(1, sizeof(struct s_main_menu));
	if (!main_menu)
		return (1);
	if (menu_create(&main_menu->menu, notcurses_stdplane(manager->nc),
		options, sizeof(options) / sizeof(options[0])))
		return (free(main_menu), MANAGER_RET_ERR("Failed to create main menu"));
	manager->stdin_handler = menu_input_handler; // Set stdin handler if needed
	manager->renderers[0] = (struct s_renderer){.render_fn = (t_renderer_fn)menu_render, .data = main_menu->menu};
	manager->renderer_count = 1;
	main_menu->menu->user_data = manager; // if menu actions need access to the manager
	*state_data = main_menu;
	return (0);
}

void	unload_main_menu_state(
	struct s_game_manager *manager,
	void *state_data
)
{
	struct s_main_menu	*main_menu = (struct s_main_menu *)state_data;

	if (!manager || !state_data)
		return ;
	if (main_menu->menu)
		menu_destroy(main_menu->menu);
	free(main_menu);
	unload_clean_all(manager);
}
