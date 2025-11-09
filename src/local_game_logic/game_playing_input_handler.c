
# include "game/game_data.h"
# include "game/game_local.h"

int swap_menu_handling(
	struct s_game_local *game,
	struct s_menu *menu,
	struct ncinput event
)
{
	if (event.id == NCKEY_UP)
		menu_select_prev(menu);
	else if (event.id == NCKEY_DOWN)
		menu_select_next(menu);
	else if (event.id == NCKEY_ENTER
		|| event.id == '\n'
		|| event.id == '\r'
	)
	{
		if (menu_activate_selected(menu))
			return (1);
	}
	else if (event.id == NCKEY_BACKSPACE)
	{
		game->player_action.action = PLAYER_ACTION_DEFAULT;
		game->player_action.ready = 1;
	}
	return (0);
}

int swap_pile_display_handling(
	struct s_game_local *game,
	struct s_pile_display *pile_display,
	struct ncinput event
)
{
	if (event.id == NCKEY_LEFT)
	{
		pile_display->selected--;
		if (pile_display->selected < 0)
			pile_display->selected = pile_display->cards->count - 1;
		game->swap_pile->is_dirty = 1;
	}
	else if (event.id == NCKEY_RIGHT)
	{
		pile_display->selected++;
		if (pile_display->selected >= (ssize_t)pile_display->cards->count)
			pile_display->selected = 0;
		game->swap_pile->is_dirty = 1;
	}
	else if (event.id == NCKEY_ENTER
		|| event.id == '\n'
		|| event.id == '\r'
	)
		game_local_select_swap_hand(game);
	else if (event.id == NCKEY_BACKSPACE)
		game_local_select_menu(game);
	return (0);
}

int swap_hand_handling(
	struct s_game_local *game,
	struct s_hand *hand,
	struct ncinput event
)
{
	struct s_cdll_node	*current;
	struct s_card_desc	hand_card;
	struct s_card_desc	pile_card;

	if (event.id == NCKEY_LEFT)
		hand_select_prev_card(hand);
	else if (event.id == NCKEY_RIGHT)
		hand_select_next_card(hand);
	else if (event.id == NCKEY_ENTER
		|| event.id == '\n'
		|| event.id == '\r'
	)
	{
		if (!hand->selected_card_plane)
			return (game_local_select_menu(game), 0);
		if (game->swap_pile->selected < 0)
			return (game_local_select_menu(game), 0);
		hand_card = hand->selected_card_plane->card_desc;
		current = game->swap_pile->cards->head;
		for (size_t i = 0; i < game->swap_pile->cards->count; i++)
		{
			if ((ssize_t)i == game->swap_pile->selected)
			{
				struct s_card_plane *card_plane;
				card_plane = current->data;
				if (card_plane)
					pile_card = card_plane->card_desc;
				break ;
			}
			current = current->next;
		}
		if (!hand->selected_card_plane || game->swap_pile->selected < 0)
			return (game_local_select_menu(game), 0);
		hand_remove_card(hand, hand_card);
		hand_add_card(hand, pile_card);
		pile_display_remove_card(game->swap_pile, pile_card);
		pile_display_add_card_top(game->swap_pile, hand_card);
		game_local_select_menu(game);
	}
	else if (event.id == NCKEY_BACKSPACE)
		game_local_select_pile(game);
	return (0);
}

int	swap_phase_handling(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct ncinput event
)
{
	(void)manager;
	switch (game->selected_item)
	{
		case SELECTED_ITEM_SWAP_MENU:
			return (swap_menu_handling(game, game->swap_menu, event));
		case SELECTED_ITEM_SWAP_PILE:
			return (swap_pile_display_handling(game, game->swap_pile, event));
		case SELECTED_ITEM_SWAP_HAND:
			return (swap_hand_handling(game, game->hand, event));
		default:
			break ;
	}
	return (0);
}

int play_phase_handling(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct ncinput event
)
{
	struct s_card_desc	card;

	(void)manager;
	if (event.id == NCKEY_ENTER
		|| event.id == '\n'
		|| event.id == '\r'
	)
	{
		if (game->whos_turn == 0 && !game->player_action.ready
			&& (game->hand->card_count > 0 || game->hand->shed_count > 0)
			&& game->hand->selected_card_plane)
		{
			card = game->hand->selected_card_plane->card_desc;
			if ((game->hand->status == HAND_DISPLAY_HAND
				&& game->hand->card_count > 0
				&& !hand_card_in_shed(game->hand, card))
				|| (game->hand->status == HAND_DISPLAY_SHED
				&& game->hand->card_count == 0
				&& hand_card_in_shed(game->hand, card)))
			{
				action_play_cards(&game->player_action, &card, 1);
				action_ready(&game->player_action);
			}
		}
	}
	else if (event.id == NCKEY_SPACE)
	{
		game_local_action_menu_init(manager, game);
		game_local_select_action_menu(game);
	}
	else if (event.id == NCKEY_LEFT) // TODO: vim controls, wasd?
		hand_select_prev_card(game->hand);
	else if (event.id == NCKEY_RIGHT)
		hand_select_next_card(game->hand);
	else if (event.id == NCKEY_UP
		|| event.id == NCKEY_DOWN)
		hand_toggle_display(game->hand);
	return (0);
}

int action_menu_handling(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_menu *menu,
	struct ncinput event
)
{
	if (event.id == NCKEY_UP)
		menu_select_prev(menu);
	else if (event.id == NCKEY_DOWN)
		menu_select_next(menu);
	else if (event.id == NCKEY_ENTER
		|| event.id == '\n'
		|| event.id == '\r'
	)
	{
		if (menu_activate_selected(menu))
			return (1);
	}
	else if (event.id == NCKEY_BACKSPACE)
	{
		game_local_select_swap_hand(game);
		game_local_action_menu_cleanup(manager, game);
	}
	return (0);
}

int log_plane_handling(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct ncinput event
)
{
	(void)manager;
	if (event.id == NCKEY_BACKSPACE)
		game->selected_item = SELECTED_ITEM_HAND;
	return (0);
}

int	game_local_play_input_handler(
	struct s_game_manager *manager,
	struct ncinput event
)
{
	struct s_game_local	*game;
	
	if (!manager)
		return (1);
	game = manager->state_data;
	if (manager->state != GAME_STATE_GAME_LOCAL_PLAY)
		return (MANAGER_RET_ERR("Not in the right state"));
	if (event.id == 'p')
		return (manager->next_state = GAME_STATE_GAME_LOCAL_PAUSE, 0);
	else
	{
		switch (game->selected_item)
		{
			case SELECTED_ITEM_HAND:
				return (play_phase_handling(manager, game, event));
			case SELECTED_ITEM_LOG:
				return (log_plane_handling(manager, game, event));
			case SELECTED_ITEM_SWAP_HAND:
			case SELECTED_ITEM_SWAP_MENU:
			case SELECTED_ITEM_SWAP_PILE:
				return (swap_phase_handling(manager, game, event));
			case SELECTED_ITEM_ACTION_MENU:
				return (action_menu_handling(manager, game, game->player_action_menu, event));
			default:
				break ;
		}
	}
	return (0);
}
