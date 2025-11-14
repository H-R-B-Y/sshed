
# include "game/game_local.h"
# include "game/game_data.h"

struct s_phase_data
{
	int	deal_first_call;
	int	deal_frame_timeout;
	int	deal_cards_dealt;
	int	swap_first_call;
	int	swap_swaps_done;
	int	swap_last_player;
};

static struct s_phase_data *staticphase_data(void)
{
	static struct s_phase_data dat;
	return (&dat);
}
int	reset_phase_data(void)
{
	struct s_phase_data *dat;

	dat = staticphase_data();
	dat->deal_first_call = 1;
	dat->deal_cards_dealt = 0;
	dat->deal_frame_timeout = FRAME_TIMEOUT;
	dat->swap_first_call = 1;
	return (0);
}

static int	deal_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	struct s_phase_data	*dat;
	struct s_card_desc	card;
	int					rand;

	if (!manager || !game)
		return (1);
	dat = staticphase_data();
	if (dat->deal_cards_dealt >= (game->settings.player_count + 1) * CARDS_PER_PLAYER)
	{
		game->play_state = PLAY_STATE_SWAP_PHASE;
		dat->deal_cards_dealt = 0;
		dat->deal_first_call = 1;
		dat->deal_frame_timeout = FRAME_TIMEOUT;
		game->whos_turn = 0; // TODO: decide who goes first (eldest hand)
		return (0);
	}
	if (dat->deal_frame_timeout > 0)
		return (dat->deal_frame_timeout--, 0);
	if (dat->deal_first_call)
	{
		ft_srand(time(NULL));
		rand = ft_rand(100, 400);
		while (rand--)
			deck_shuffle(game->deck, rand);
		hand_show_shed(game->hand);
		for (t_u8 p = 0; p < game->settings.player_count; p++)
			pdisplay_show_shed(game->pdisplay[p]);
		dat->deal_first_call = 0;
	}
	dat->deal_frame_timeout = FRAME_TIMEOUT;
	for (t_u8 i = 0; i < game->settings.player_count + 1; i++)
	{
		if (deck_display_draw_top_card(game->deck_display, &card))
			return (1);
		dat->deal_cards_dealt++;
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
				if (manager->settings.auto_sort)
					hand_sort_cards(game->hand);
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
	struct s_phase_data	*dat;
	// In the swap phase we want to allow the player to swap a certain number of cards
	// from the face up cards in the shed to their hand 
	// To do this we will use a menu and a pile
	// the pile display will show the cards from the face up shed cards
	// the menu will allow the player to select which cards to swap
	// Once the player has made their selection we will swap the cards
	// and then move to the play phase
	dat = staticphase_data();
	if (!manager || !game)
		return (1);
	if (dat->swap_first_call)
	{
		dat->swap_last_player = game->whos_turn;
		init_swap_phase(manager, game);
		deck_display_hide(game->deck_display);
		game_local_select_menu(game);
		re_order_visuals(manager, game);
		if (game->whos_turn != 0)
			game_local_select_nothing(game);
		dat->swap_first_call = 0;
		return (0);
	}
	if (dat->swap_last_player != game->whos_turn)
	{
		dat->swap_last_player = game->whos_turn;
		if (dat->swap_last_player == 0)
			game_local_select_menu(game);
		else
			game_local_select_nothing(game);
	}
	if (game->player_action.ready)
	{
		dat->swap_swaps_done++;
		if (game->whos_turn == 0)
		{
			while (game->swap_pile->cards->count)
			{
				struct s_card_desc card; 
				card = ((struct s_card_plane *)game->swap_pile->cards->head->data)->card_desc;
				pile_display_remove_card(game->swap_pile, card);
				hand_add_card_to_shed(game->hand, card);
			}
		}
		game_local_increment_whos_turn(game);
		game->player_action = clean_action();
		if (dat->swap_swaps_done >= game->settings.player_count + 1)
		{
			game->play_state = PLAY_STATE_PLAY_PHASE;
			unload_swap_phase(manager, game);
			game->selected_item = SELECTED_ITEM_HAND;
			game->hand->card_selected[0] = 0;
			hand_update_selected(game->hand);
			game->hand->hand_dirty = 1;
			deck_display_show(game->deck_display);
			dat->swap_swaps_done = 0;
			dat->swap_first_call = 1;
		}
	}
	return (0);
}

static int	play_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	int					who_won;

	if (!manager || !game)
		return (1);
	if (game->player_action.ready)
	{
		if (game->player_action.action == PLAYER_ACTION_NONE)
			return (game->player_action.ready = 0, 0);
		if (_handle_player_action(manager, game, &game->player_action))
			return (MANAGER_RET_ERR("Player action caused an error"));
		game_local_increment_whos_turn(game);
		game->player_action = clean_action();
		who_won = game_local_who_won(game);
		if (who_won != -1)
			(game->play_state = PLAY_STATE_GAME_END, game->who_won = who_won);
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