
# include "game/game_data.h"
# include "game/game_local.h"

int swap_menu_handling(
	struct s_game_local *game,
	struct s_menu *menu,
	t_u32 c_code
)
{
	if (c_code == NCKEY_UP)
	{
		menu_select_prev(menu);
	}
	else if (c_code == NCKEY_DOWN)
	{
		menu_select_next(menu);
	}
	else if (c_code == NCKEY_ENTER
		|| c_code == '\n'
		|| c_code == '\r'
	)
	{
		if (menu_activate_selected(menu))
			return (1);
	}
	else if (c_code == NCKEY_BACKSPACE)
	{
		game->player_action.action = PLAYER_ACTION_DEFAULT;
		game->player_action.ready = 1;
	}
	return (0);
}

int swap_pile_display_handling(
	struct s_game_local *game,
	struct s_pile_display *pile_display,
	t_u32 c_code
)
{
	if (c_code == NCKEY_LEFT)
	{
		pile_display->selected--;
		if (pile_display->selected < 0)
			pile_display->selected = pile_display->cards->count - 1;
		game->swap_pile->is_dirty = 1;
	}
	else if (c_code == NCKEY_RIGHT)
	{
		pile_display->selected++;
		if (pile_display->selected >= (ssize_t)pile_display->cards->count)
			pile_display->selected = 0;
		game->swap_pile->is_dirty = 1;
	}
	else if (c_code == NCKEY_ENTER
		|| c_code == '\n'
		|| c_code == '\r'
	)
	{
		swap_phase_select_hand(game);
	}
	else if (c_code == NCKEY_BACKSPACE)
	{
		swap_phase_select_menu(game);
	}
	return (0);
}

int swap_hand_handling(
	struct s_game_local *game,
	struct s_hand *hand,
	t_u32 c_code
)
{
	if (c_code == NCKEY_LEFT)
	{
		hand_select_prev_card(hand);
	}
	else if (c_code == NCKEY_RIGHT)
	{
		hand_select_next_card(hand);
	}
	else if (c_code == NCKEY_ENTER
		|| c_code == '\n'
		|| c_code == '\r'
	)
	{
		struct s_card_desc	hand_card;
		struct s_card_desc	pile_card;

		if (!hand->selected_card_plane)
		{
			swap_phase_select_menu(game);
			return (0);
		}
		if (game->swap_pile->selected < 0)
		{
			swap_phase_select_menu(game);
			return (0);
		}
		hand_card = hand->selected_card_plane->card_desc;
		// Need to get the card from the pile display
		struct s_cdll_node *current;
		current = game->swap_pile->cards->head;
		for (int i = 0; i < game->swap_pile->cards->count; i++)
		{
			if (i == game->swap_pile->selected)
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
		{
			swap_phase_select_menu(game);
			return (0);
		}
		hand_remove_card(hand, hand_card);
		hand_add_card(hand, pile_card);
		pile_display_remove_card(game->swap_pile, pile_card);
		pile_display_add_card_top(game->swap_pile, hand_card);
		swap_phase_select_menu(game);
	}
	else if (c_code == NCKEY_BACKSPACE)
	{
		swap_phase_select_pile(game);
	}
	return (0);
}

int	swap_phase_handling(
	struct s_game_manager *manager,
	struct s_game_local *game,
	t_u32 c_code
)
{
	switch (game->selected_item)
	{
		case SELECTED_ITEM_SWAP_MENU:
			return swap_menu_handling(game, game->swap_menu, c_code);
		case SELECTED_ITEM_SWAP_PILE:
			return swap_pile_display_handling(game, game->swap_pile, c_code);
		case SELECTED_ITEM_SWAP_HAND:
			return swap_hand_handling(game, game->hand, c_code);
		default:
			break ;
	}
	return (0);
}

int play_phase_handling(
	struct s_game_manager *manager,
	struct s_game_local *game,
	t_u32 c_code
)
{
	struct s_card_desc	card;

	if (c_code == NCKEY_ENTER
		|| c_code == '\n'
		|| c_code == '\r'
		|| c_code == NCKEY_SPACE
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
				game->player_action.action = PLAYER_ACTION_PLAY;
				game->player_action.cards[0] = card;
				game->player_action.card_count = 1;
				game->player_action.ready = 1; // TODO: update this
			}
		}
	}
	else if (c_code == NCKEY_LEFT) // TODO: vim controls, wasd?
	{
		hand_select_prev_card(game->hand);
	}
	else if (c_code == NCKEY_RIGHT)
	{
		hand_select_next_card(game->hand);
	}
	else if (c_code == NCKEY_UP
		|| c_code == NCKEY_DOWN)
	{
		hand_toggle_display(game->hand);
	}
	return (0);
}

int log_plane_handling(
	struct s_game_manager *manager,
	struct s_game_local *game,
	t_u32 c_code
)
{
	if (c_code == NCKEY_BACKSPACE)
	{
		game->selected_item = SELECTED_ITEM_HAND;
	}
	return (0);
}

int	game_local_play_input_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
)
{
	struct s_game_local	*game;
	t_u32				c_code;
	
	if (!manager || !event)
		return (1);
	game = manager->state_data;
	if (manager->state != GAME_STATE_GAME_LOCAL_PLAY)
		return (MANAGER_RET_ERR("Not in the right state"));
	// So there are some keys that need to happen right away like pause
	// or some other stuff, else we fallback to passing the keys to the hand manager
	if (manager->reading_fd != event->data.fd)
		return (MANAGER_RET_ERR("Wrong fd for reading"));
	c_code = notcurses_get_blocking(manager->nc, NULL);
	if (c_code == 'p')
	{
		manager->next_state = GAME_STATE_GAME_LOCAL_PAUSE;
		return (0);
	}
	else if (c_code == NCKEY_TAB)
		notcurses_refresh(manager->nc, NULL, NULL);
	else
	{
		switch (game->selected_item)
		{
			case SELECTED_ITEM_HAND:
				return play_phase_handling(manager, game, c_code);
			case SELECTED_ITEM_LOG:
				return log_plane_handling(manager, game, c_code);
			case SELECTED_ITEM_SWAP_HAND:
			case SELECTED_ITEM_SWAP_MENU:
			case SELECTED_ITEM_SWAP_PILE:
				return swap_phase_handling(manager, game, c_code);
			default:
				break ;
		}
	}
	return (0);
}
