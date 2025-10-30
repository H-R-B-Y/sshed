
# include "game/game_data.h"
# include "game/game_local.h"

static int	menu_input_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
)
{
	struct s_game_local_end	*main_menu;
	struct s_menu			*menu;

	if (!manager || !manager->state_data || !event)
		return (1);
	main_menu = (struct s_game_local_end *)manager->state_data;
	if (!main_menu)
		return (1);
	menu = main_menu->menu;
	if (event->data.fd != manager->reading_fd)
		return (0);
	return (_menu_key_handler(menu, manager->nc));
}

// TODO: Really this could be a static text option that is created in the load function but we can do that later
char	*who_won_text(struct s_menu *menu)
{
	char					*temp;
	char					*output;
	struct s_game_local_end	*state;
	struct s_game_manager	*manager;

	if (!menu || !menu->user_data)
		return (NULL);
	manager = menu->user_data;
	state = manager->state_data;
	switch (state->who_won)
	{
		case (3):
		case (2):
		case (1):
			temp = "Computer";
			break ;
		default:
			temp = "You";
			break ;
	}
	output = ft_calloc(32, sizeof(char));
	if (!output)
		return (NULL);
	sprintf(output, "%s won!", temp);
	return (output);
}

int		return_to_menu(struct s_menu *menu, struct notcurses *nc)
{
	struct s_game_manager	*manager;

	if (!menu || !nc || !menu->user_data)
		return (1);
	manager = menu->user_data;
	manager->next_state = GAME_STATE_MAIN;
	return (0);
}

int		load_local_end(struct s_game_manager *manager, void **state_data)
{
	struct s_game_local_end	*end_state;
	struct s_game_local		*game_state;
	struct s_menu_option	options[] = {
		{.text_type = DYNAMIC_TEXT_FUNCTION, .get_option_text = who_won_text, .option_action = NULL},
		{.text_type = STATIC_TEXT, .option_text = "Continue to menu", .option_action = return_to_menu}
	};

	if (!manager || !state_data)
		return (1);
	// When we are loading into this state we assume that the
	// game state data is stored in the previous state
	// if not that is ok we will just have to display a generic win screen
	end_state = ft_calloc(1, sizeof(struct s_game_local_end));
	if (!end_state)
		return (1);
	game_state = manager->prev_state_data;
	if (game_state)
	{
		end_state->who_won = game_state->who_won;
		end_state->log_data = game_state->game_log;
		game_state->game_log = NULL;
		manager->prev_state_data_destructor(game_state);
		manager->prev_state_data = NULL;
		manager->prev_state_data_destructor = NULL;
	}
	// Then create the end menu
	if (menu_create(&end_state->menu,
		notcurses_stdplane(manager->nc),
		options,
		sizeof(options) / sizeof(options[0])
		))
		return ((manager->errmsg = "Failed to create the end screen menu"), free_local_end(end_state), 1);
	end_state->menu->user_data = manager;
	manager->renderers[0].data = end_state->menu;
	manager->renderers[0].render_fn = (t_renderer_fn)menu_render;
	manager->stdin_handler = menu_input_handler;
	manager->renderer_count = 1; // TODO: this should be two one for the menu and one for the log plane;
	(*state_data) = end_state;
	return (0);
}

void	unload_local_end(struct s_game_manager *manager, void *state_data)
{
	struct s_game_local_end	*end_state;

	if (!manager||!state_data)
		return ;
	end_state = state_data;
	if (end_state->menu)
		menu_destroy(end_state->menu);
	free(end_state);
	manager->state_data = NULL;
	manager->state_data_destructor = NULL;
	manager->prev_state_data = NULL;
	manager->prev_state_data_destructor = NULL;
	manager->stdin_handler = NULL;
}

void	free_local_end(struct s_game_local_end *end_state)
{
	(void)end_state;
}

