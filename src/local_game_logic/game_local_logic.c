
# include "game/game_local.h"
# include "game/game_data.h"

# define FRAME_TIMEOUT 1
# define CARDS_PER_PLAYER (3 + 3 + 5) // 3 face down, 3 face up, 5 in hand

static int	deal_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	static int			cards_dealt = 0;
	static int			frame_countdown = FRAME_TIMEOUT;
	static int			first_call = 1;
	struct s_card_desc	card;

	if (!manager || !game)
		return (1);
	if (cards_dealt >= (game->settings.player_count + 1) * CARDS_PER_PLAYER)
	{
		// game->play_state = PLAY_STATE_SWAP_PHASE;
		game->play_state = PLAY_STATE_PLAY_PHASE; // swap phase not implemented yet
		cards_dealt = 0;
		first_call = 1;
		frame_countdown = FRAME_TIMEOUT;
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
	frame_countdown = FRAME_TIMEOUT;
	for (t_u8 i = 0; i < game->settings.player_count + 1; i++)
	{
		if (deck_display_draw_top_card(game->deck_display, &card))
			return (1);
		cards_dealt++;
		if (i == 0)
		{
			if (game->hand->shed_count < 6)
			{
				if (hand_add_card_to_shed(game->hand, card))
					return (1);
			}
			else
			{
				if (game->hand->status == HAND_DISPLAY_SHED)
					hand_show_hand(game->hand);
				if (hand_add_card(game->hand, card))
					return (1);
			}
		}
		else
		{
			if (game->pdisplay[i - 1]->shed_count < 6)
			{
				if (pdisplay_add_card_shed(game->pdisplay[i - 1], card))
					return (1);
			}
			else
			{
				if ((game->pdisplay[i - 1])->status == PDISPLAY_SHED)
					pdisplay_show_hand(game->pdisplay[i - 1]);
				if (pdisplay_add_card(game->pdisplay[i - 1], card))
					return (1);
			}
		}
	}
	return (0);
}

static int	swap_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	(void)manager;(void)game;
	// Not implemented yet
	// How do we do this
	/*
	We need to add a new kind of display that allows the user
	to select both a card in their hand and a card from the shed (face up)
	so we will need some new kind of visual, maybe one that is created in load
	we could also use this visual to allow players to play multiple cards
	So it would need to be a pile, the AI players dont need this, they just need to
	be aware of the game phase and return values that relate to swapping the cards
	*/
	return (0);
}

static int	play_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	static int			cards_played = 0;
	int					who_won;
	/*
	When we are in the play phase we want to check for
	win conditions, we dont need to do this every frame, that is too expensive
	so maybe we could signal if a player has finished their turn recently
	*/
	if (!manager || !game)
		return (1);

	if (game->card_has_been_played)
	{
		// TODO: This is really the next bit that needs to happen:
		// This is where we move the card from the hand to the play pile
		// But we will need to check some things
		// - Does the player actually own this card?
		// - Is the card in the shed or the hand
		// - Is the card in a playable state (not from shed if still cards in the hand)
		// - Is the card playable on the top of the pile if so
		//		- Pop the card from the pdisplay/hand
		//		- Add the card to the pile
		// - Otherwise
		//		- Return the card to the hand/pdisplay
		//		- Move all cards in the pile to the hand/pdisplay

		/*
		Next steps
			- Functions for hand and pdisplay to check if a card is owned
			- Functions for hand and display to check if a card is either in the hand or the shed
			- Functions for checking if a card is playable on the top of the pile
		*/

	}

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

int	end_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	// In the end phase we want to stop the AI
	// We want to do anything that wont be done in the unload function
	// then we want to change the manager state to end state
	if (!manager || !game)
		return (1);
	manager->next_state = GAME_STATE_GAME_LOCAL_END;
	return (0);
}

int	pre_render_game_update(struct s_game_manager *manager)
{
	struct s_game_local	*game;
	int					ret;
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
			ret = deal_phase(manager, game);
			break ;
		case (PLAY_STATE_SWAP_PHASE):
			ret = swap_phase(manager, game);
			break ;
		case (PLAY_STATE_PLAY_PHASE):
			ret = play_phase(manager, game);
			break ;
		case (PLAY_STATE_GAME_END):
			ret = end_phase(manager, game);
			break ;
		case (PLAY_STATE_ERROR):
			// dprintf(STDERR_FILENO, "ERROR has occured in the update loop\n");
			manager->errmsg = "Fatal error in the game update logic";
			ret = 1;
			break ;
		default:
			manager->errmsg = "Unmatched game phase";
			ret = 1;
			break ;
	}
	return (ret);
}