
# include "game/game_local.h"
# include "game/game_data.h"

int init_swap_phase(struct s_game_manager *manager, struct s_game_local *game);

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
		game->play_state = PLAY_STATE_SWAP_PHASE;
		// game->play_state = PLAY_STATE_PLAY_PHASE; // swap phase not implemented yet
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
		ft_srand(time(NULL));
		int i = ft_rand(10, 100);
		while (i--)
		{
			deck_shuffle(game->deck, i);
		}
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
	// TODO: When we have dealt all of the cards, we need to descide who goes first
	// Need to use eldest hand rule for accuracy, but could be changed in the game rules
	return (0);
}


int unload_swap_phase(struct s_game_manager *manager, struct s_game_local *game);

static int	swap_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	// In the swap phase we want to allow the player to swap a certain number of cards
	// from the face up cards in the shed to their hand 
	// To do this we will use a menu and a pile
	// the pile display will show the cards from the face up shed cards
	// the menu will allow the player to select which cards to swap
	// Once the player has made their selection we will swap the cards
	// and then move to the play phase

	if (!manager || !game)
		return (1);
	if (!game->swap_menu || !game->swap_pile)
	{
		init_swap_phase(manager, game);
		deck_display_hide(game->deck_display);
		swap_phase_select_menu(game);
		re_order_visuals(manager, game);
		return (0);
	}
	if (game->player_action.ready)
	{
		// Return cards to shed
		while (game->swap_pile->cards->count)
		{
			struct s_card_desc card; 
			card = ((struct s_card_plane *)game->swap_pile->cards->head->data)->card_desc;
			pile_display_remove_card(game->swap_pile, card);
			hand_add_card_to_shed(game->hand, card);
		}
		game->whos_turn++;
		game->player_action = clean_action();
		if (game->whos_turn > game->settings.player_count)
		{
			game->play_state = PLAY_STATE_PLAY_PHASE;
			game->whos_turn = 0;//decide who goes first at some point
			unload_swap_phase(manager, game);
			game->selected_item = SELECTED_ITEM_HAND;
			deck_display_show(game->deck_display);
		}
	}
	return (0);
}

static int	play_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	int					who_won;
	/*
	When we are in the play phase we want to check for
	win conditions, we dont need to do this every frame, that is too expensive
	so maybe we could signal if a player has finished their turn recently
	*/
	if (!manager || !game)
		return (1);

	if (game->player_action.ready)
	{
		if (game->player_action.action == PLAYER_ACTION_NONE)
		{
			game->player_action.ready = 0;
			return (0);
		}
		if (_handle_player_action(manager, game, &game->player_action))
			return MANAGER_RET_ERR("Player action caused an error");
		// Player turn went through, and side effects of their action has been dealt with
		// Increment who's turn it is
		game->whos_turn = (game->whos_turn + 1);
		if (game->whos_turn > game->settings.player_count)
			game->whos_turn = 0;
		game->player_action = clean_action();
		who_won = -1;

		// A player has played a card since last frame
		// cards_played = game->cards_played;
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
		{
			game->play_state = PLAY_STATE_GAME_END;
			game->who_won = who_won;
		}
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
			ret = MANAGER_RET_ERR("Fatal error in the game update logic");
			break ;
		default:
			ret = MANAGER_RET_ERR("Unmatched game phase");
			break ;
	}
	return (ret);
}