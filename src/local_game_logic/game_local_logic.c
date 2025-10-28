
# include "game_local.h"
# include "game_data.h"

# define CARDS_PER_PLAYER (3 + 3 + 5) // 3 face down, 3 face up, 5 in hand

int	deal_phase(struct s_game_manager *manager)
{
	static int			cards_dealt = 0;
	static int			frame_countdown = 10;
	static int			first_call = 1;
	struct s_game_local	*game;
	struct s_card_desc	card;

	if (!manager)
		return (1);
	game = manager->state_data;
	if (!game || manager->state != GAME_STATE_GAME_LOCAL_PLAY)
		return (1);
	if (cards_dealt >= (game->settings.player_count + 1) * CARDS_PER_PLAYER)
	{
		// game->play_state = PLAY_STATE_SWAP_PHASE;
		game->play_state = PLAY_STATE_PLAY_PHASE; // swap phase not implemented yet
		cards_dealt = 0;
		first_call = 1;
		frame_countdown = 10;
		return (0);
	}
	if (frame_countdown > 0)
	{
		frame_countdown--;
		return (0);
	}
	// Deal a card to each player
	/*
	pseudo-async dealing here
	we will have this called every frame so 60hz right now
	we will deal a card every 10 frames
	We want to make sure that each players starts with their
	shed view open and visible so
	*/
	if (first_call)
	{
		hand_show_shed(game->hand);
		for (t_u8 i = 0; i < game->settings.player_count; i++)
			pdisplay_show_shed(game->pdisplay[i]);
		first_call = 0;
	}
	frame_countdown = 10;
	for (t_u8 i = 0; i < game->settings.player_count + 1; i++)
	{
		if (deck_draw_card(game->deck, &card))
			return (1);
		cards_dealt++;
		if (i == 0)
		{
			if (game->hand->shed_count < 6 && hand_add_card_to_shed(game->hand, card))
				return (1);
			else if (hand_add_card_to_hand(game->hand, card))
				return (1);
		}
		else
		{
			if (game->pdisplay[i - 1]->shed_count < 6
				&& pdisplay_add_card_shed(game->pdisplay[i - 1], card))
				return (1);
			else if (pdisplay_add_card_hand(game->pdisplay[i - 1], card))
				return (1);
		}
		return (0);
	}
}

int	swap_phase(struct s_game_manager *manager)
{
 // Not implemented yet
}

int	play_phase(struct s_game_manager *manager)
{
	static int			cards_played = 0;
	struct s_game_local	*game;
	int					who_won;
	/*
	When we are in the play phase we want to check for
	win conditions, we dont need to do this every frame, that is too expensive
	so maybe we could signal if a player has finished their turn recently
	*/
	if (!manager)
		return (1);
	game = manager->state_data;
	if (!game || manager->state != GAME_STATE_GAME_LOCAL_PLAY)
		return (1);
	who_won = -1;
	if (game->cards_played != cards_played)
	{
		// A player has played a card since last frame
		cards_played = game->cards_played;
		// Check for win conditions here
		if (game->hand->card_count == 0 && game->hand->shed_count == 0)
			who_won = 0;
		if (who_won == -1)
		{
			for (t_u8 i = 0; i < game->pdisplay_count; i++)
			{
				if (game->pdisplay[i]->card_count == 0
					&& game->pdisplay[i]->shed_count == 0)
				{
					who_won = i + 1;
					break ;
				}
			}
		}
		if (who_won != -1)
			game->play_state = PLAY_STATE_GAME_END;
	}
	return (0);
}

int	end_phase(struct s_game_manager *manager)
{
	// In the end phase we want to stop the AI
	// We want to do anything that wont be done in the unload function
	// then we want to change the manager state to end state
	if (!manager)
		return (1);
	manager->next_state = GAME_STATE_GAME_LOCAL_END;
	return (0);
}

int	pre_render_game_update(struct s_game_manager *manager)
{
	struct s_game_local	*game;
	static int			cards_dealt = 0;
	/*
	Here we want to check if any win state has been achieved
	We want to check the current state and see if any transitions need to happen
	There should also be some function that is available for each turn
	player or AI we need to avaluate if the turn they took was valid
	and if they need to pick up the pile, etc..
	We want to deal cards
	etc..
	*/
	if (!manager)
		return (1);
	game = manager->state_data;
	if (!game || manager->state != GAME_STATE_GAME_LOCAL_PLAY)
		return (1);
	switch (game->play_state)
	{
		case (PLAY_STATE_DEAL_PHASE):
			break ;
		case (PLAY_STATE_SWAP_PHASE):
			break ;
		case (PLAY_STATE_PLAY_PHASE):
			break ;
		case (PLAY_STATE_GAME_END):
			break ;
		case (PLAY_STATE_ERROR):
			break ;
		default:
			break ;
	}
	return (0);
}