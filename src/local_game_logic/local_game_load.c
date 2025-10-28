
# include "game_local.h"
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
	ncplane_erase(game->log_plane);
	if (!game->log_visible)
		return (0);
	ncplane_dim_yx(game->log_plane, &height, &width);
	// TODO: want to write the log messages to the log plane

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
		&(game->ai_data[game->whos_turn - 1]),
		game->pdisplay[game->whos_turn - 1],
		game->pile_display
	);
	switch (ai_state)
	{
		case (AI_STATE_DONE):
			game->whos_turn++;
			if (game->whos_turn > game->pdisplay_count + 1)
				game->whos_turn = 0;
			return (0);
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
		5, 0, 0))
		return ((manager->errmsg = "Failed to create pile display"), 1);
	return (0);
}


int	load_game_local(
	struct s_game_manager *manager,
	void **state_data
)
{
	struct s_game_local	*game;

	if (!manager || !state_data)
		return (1);
	game = ft_calloc(1, sizeof(struct s_game_local));
	if (!game)
		return ((manager->errmsg = "Failed to allocated game state"), 1);
	if (!manager->prev_state_data)
		((manager->errmsg = "No config provided, using default"), (game->settings = DEFAULT_SETTINGS));
	else
		game->settings = *((struct s_game_local_settings *)manager->prev_state_data);
	(*state_data) = game;
	if (init_log_plane(manager, game))
	{
		// TODO: cleanup
		return (1);
	}
	game->deck = deck_create(true);
	if (load_visuals(manager, game))
	{
		// TODO: cleanup
		return (1);
	}
	// Ok so we have the visuals initialised
	// calculate who's turn it is

	// Setup the renderers
	// There should be a function somewhere to perform the player tasks
	manager->renderers[0].render_fn = pre_render_ai_update;
	manager->renderers[0].data = manager;
	manager->renderers[1].render_fn = hand_render;
	manager->renderers[1].data = game->hand;
	manager->renderers[2].render_fn = (t_renderer_fn)render_pdisplays;
	manager->renderers[2].data = game;
	manager->renderers[3].render_fn = (t_renderer_fn)pile_display_render;
	manager->renderers[3].data = game->pdisplay;
	manager->renderers[4].render_fn = (t_renderer_fn)deck_display_render;
	manager->renderers[4].data = game->deck_display;
	manager->renderers[5].render_fn = render_log_display;
	manager->renderers[5].data = game;
	manager->renderer_count = 6;

	game->play_state = PLAY_STATE_DEAL_PHASE; // TODO: figure out swap phase

	return (0);
}

// Unload function

