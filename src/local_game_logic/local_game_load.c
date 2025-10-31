
# include "game/game_local.h"
# include "game/game_data.h"

# define DEFAULT_SETTINGS (struct s_game_local_settings){.player_count = 3}
// Load function


int	render_pdisplays(struct s_game_local *game)
{
	unsigned int	idx;

	if (!game)
		return (1);
	idx = 0;
	while (idx < game->pdisplay_count)
	{
		if (pdisplay_render(game->pdisplay[idx]))
			return (1);
		idx++;
	}
	return (0);
}

int	render_log_display(struct s_game_local *game)
{
	unsigned int	width, height;

	if (!game)
		return (1);
	if (!game->log_plane)
		return (0);
	ncplane_erase(game->log_plane);
	if (!game->log_visible)
		return (0);
	ncplane_dim_yx(game->log_plane, &height, &width);
	// TODO: want to write the log messages to the log plane
	return (0);
}

int	pre_render_ai_update(struct s_game_manager *manager)
{
	t_ai_state			ai_state;
	struct s_game_local	*game;

	if (!manager)
		return (1);
	game = manager->state_data;
	if (!game || manager->state != GAME_STATE_GAME_LOCAL_PLAY)
		return (1);
	if (game->whos_turn == 0)
		return (0);
	ai_state = ai_step(
		&game->player_action,
		&(game->ai_data[game->whos_turn - 1]),
		game->pdisplay[game->whos_turn - 1],
		game->pile_display
	);
	switch (ai_state)
	{
		case (AI_STATE_FUCK_FUCK_FUCK):
			manager->errmsg = "AI Fatal error occured";
			return (1);
		default:
			return (0);
	}
	return (0);
}



int	init_log_plane(struct s_game_manager *manager, struct s_game_local *game)
{
	if (!manager || !game)
		return (1);
	game->max_log_line = 1;
	game->log_plane = ncplane_create(notcurses_stdplane(manager->nc), &(struct ncplane_options){0}); // TODO figure out the popts for this plane
	return (0);
}

int	load_visuals(struct s_game_manager *manager,struct s_game_local *game)
{
	unsigned int	i;

	if (deck_display_create(&game->deck_display, notcurses_stdplane(manager->nc), game->deck, 0, 0))
		return ((manager->errmsg = "Failed to create deck display"), 1);
	if (hand_create(&game->hand, notcurses_stdplane(manager->nc)))
		return ((manager->errmsg = "Failed to create player hand"), 1);
	game->pdisplay_count = game->settings.player_count;
	i = 0;
	while (i < game->pdisplay_count)
	{
		if (pdisplay_create(
			&game->pdisplay[i],
			notcurses_stdplane(manager->nc),
			((enum e_pdisplay_orientation[3]){PDISPLAY_ORIENTATION_TOP, PDISPLAY_ORIENTATION_LEFT, PDISPLAY_ORIENTATION_RIGHT})[i]
			))
			return ((manager->errmsg = "Failed to create a pdisplay"), 1);
		i++;
	}
	if (pile_display_create(
		&game->pile_display,
		notcurses_stdplane(manager->nc),
		CARD_WIDTH,
		CARD_ORIENTATION_VERTICAL, PILE_DISPLAY_HORIZONTAL,
		5, CARD_WIDTH + 5, CARD_HEIGHT * 2))
		return ((manager->errmsg = "Failed to create pile display"), 1);
	return (0);
}

// oh you dirty function, how naughty
static void	_make_dirty(struct s_game_local *game)
{
	if (!game)
		return ;
	if (game->hand)
		game->hand->hand_dirty = 1;
	if (game->deck_display)
		game->deck_display->is_dirty = 1;
	if (game->pile_display)
		game->pile_display->is_dirty = 1;
	for (int i = 0; i < game->pdisplay_count; i++)
	{
		if (game->pdisplay[i])
			game->pdisplay[i]->pdisplay_dirty = 1;
	}
	return ;
}

static void	_hooks(
	struct s_game_manager *manager,
	struct s_game_local *game
)
{
	manager->stdin_handler = game_local_play_input_handler;
	manager->renderers[0].render_fn = (t_renderer_fn)pre_render_game_update;
	manager->renderers[0].data = manager;
	manager->renderers[0 + 1].render_fn = (t_renderer_fn)pre_render_ai_update;
	manager->renderers[0 + 1].data = manager;
	manager->renderers[1 + 1].render_fn = (t_renderer_fn)hand_render;
	manager->renderers[1 + 1].data = game->hand;
	manager->renderers[2 + 1].render_fn = (t_renderer_fn)render_pdisplays;
	manager->renderers[2 + 1].data = game;
	manager->renderers[3 + 1].render_fn = (t_renderer_fn)pile_display_render;
	manager->renderers[3 + 1].data = game->pile_display;
	manager->renderers[4 + 1].render_fn = (t_renderer_fn)deck_display_render;
	manager->renderers[4 + 1].data = game->deck_display;
	manager->renderers[5 + 1].render_fn = (t_renderer_fn)render_log_display;
	manager->renderers[5 + 1].data = game;
	manager->renderer_count = 6;
}

int	load_game_local(
	struct s_game_manager *manager,
	void **state_data
)
{
	struct s_game_local	*game;

	if (!manager || !state_data)
		return (1);
	if (manager->state_data) // This allows us to continue from a paused state
	{
		game = manager->state_data;
		if (game->initialised)
		{
			_hooks(manager, game);
			_make_dirty(game);
			(*state_data) = game;
			return (0);
		}
		free_game_state(game);
		manager->state_data = NULL;
	}
	game = ft_calloc(1, sizeof(struct s_game_local));
	if (!game)
		return ((manager->errmsg = "Failed to allocated game state"), 1);
	if (manager->prev_state == GAME_STATE_GAME_LOCAL_SETUP && manager->prev_state_data)
	{
		game->settings = *((struct s_game_local_settings *)manager->prev_state_data);
		if (manager->prev_state_data_destructor)
			manager->prev_state_data_destructor(manager->prev_state_data);
		else
			free(manager->prev_state_data);
	}
	else
		((manager->errmsg = "No config provided, using default"), (game->settings = DEFAULT_SETTINGS));
	game->pdisplay_count = game->settings.player_count;
	(*state_data) = game;
	if (init_log_plane(manager, game))
	{
		free_game_state(game);
		return (1);
	}
	game->deck = deck_create(true);
	if (load_visuals(manager, game))
	{
		free_game_state(game);
		return (1);
	}
	// calculate who's turn it is
	// No this should be calculated in the dealing phase as we need to go with the 
	// eldest hand rules
	_hooks(manager, game);
	game->play_state = PLAY_STATE_DEAL_PHASE; // TODO: figure out swap phase
	(*state_data) = game;
	manager->state_data_destructor = (t_freefn)free_game_state;
	game->initialised = 1;
	return (0);
}

// Unload function

static void	_clear_game_display(
	struct s_game_local *game
)
{
	if (!game)
		return ;
	hand_clear_screen(game->hand);
	pile_display_clear_screen(game->pile_display);
	deck_display_clear_screen(game->deck_display);
	for (int i = 0; i < game->pdisplay_count; i++)
		pdisplay_clear_screen(game->pdisplay[i]);
	return ;
}

void	unload_game_local(
	struct s_game_manager *manager,
	void *state_data
)
{
	if (!manager || !state_data)
		return ;
	/*
	Unloading the state for a game does not delete it,
	we just move the state into the prev state data pointer
	then we need to ensure that there is a state data destructor pointer
	because game state will not be destroyed
	*/
	/*
	Checklist:
	- Free any data allocated in the load state (not applicable here)
	- Unset manager stdin handler (applicable)
	- Nullify the current state
	*/
	// TODO: we need to clear the screen here as we transition out of the state
	_clear_game_display(state_data);
	unload_unset_stdinhandler(manager);
	unload_unset_renderers(manager);
	unload_set_prev_data(manager, state_data, (t_freefn)free_game_state);
	return ;
}

// Need a function that will cleanup the actual game state

void	free_game_state(
	struct s_game_local *game
)
{
	if (!game)
		return ;
	// Free the log plane
	if (game->log_plane)
		ncplane_destroy(game->log_plane);
	game->log_plane = NULL;
	// Free the log
	if (game->game_log)
		ft_lstclear(&game->game_log, free);
	game->game_log = NULL;
	// Delete the player
	if (game->hand)
		hand_destroy(game->hand); // This will free the card planes in the hand
	game->hand = NULL;
	// Free the pdisplays
	if (game->pdisplay[0])
		for (int i = 0; i < game->settings.player_count; i++)
			pdisplay_destroy(game->pdisplay[i]); // Free's the card planes
	ft_memset(game->pdisplay, 0, sizeof(struct s_pdisplay *) * 3);
	// free the deck display
	if (game->deck_display)
		deck_display_destroy(game->deck_display);
	game->deck_display = NULL;
	// Free the pile
	if (game->pile_display)
		pile_display_destroy(game->pile_display);
	game->pile_display = NULL;
	// Free the deck
	if (game->deck)
		deck_destroy(game->deck);
	game->deck_display = NULL;
	// What else
	// ??
	// Free the pointer
	free(game);
}
