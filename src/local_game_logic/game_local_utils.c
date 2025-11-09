
# include "game/game_local.h"
# include "game/game_data.h"

int game_local_select_menu(struct s_game_local *game)
{
	if (!game)
		return (1);
	if (game->swap_menu)
		(game->selected_item = SELECTED_ITEM_SWAP_MENU, menu_show(game->swap_menu));
	if (game->swap_pile)
		(game->swap_pile->selected = -1, game->swap_pile->is_dirty = 1);
	if (game->hand)
		(game->hand->card_selected[0] = -1, game->hand->card_selected[1] = -1, hand_update_selected(game->hand), game->hand->hand_dirty = 1);
	return (0);
}

int game_local_select_pile(struct s_game_local *game)
{
	if (!game)
		return (1);
	if (game->swap_menu)
		menu_hide(game->swap_menu);
	if (game->swap_pile)
		(game->selected_item = SELECTED_ITEM_SWAP_PILE, game->swap_pile->selected = 0, game->swap_pile->is_dirty = 1);
	if (game->hand)
		(game->hand->card_selected[0] = -1, game->hand->card_selected[1] = -1, hand_update_selected(game->hand), game->hand->hand_dirty = 1);
	return (0);
}

int game_local_select_swap_hand(struct s_game_local *game)
{
	if (!game)
		return (1);
	if (game->swap_menu)
		menu_hide(game->swap_menu);
	if (game->swap_pile)
		(game->swap_pile->is_dirty = 1);
	if (game->hand)
		(game->selected_item = SELECTED_ITEM_SWAP_HAND, game->hand->card_selected[0] = 0, game->hand->card_selected[1] = -1, hand_update_selected(game->hand), game->hand->hand_dirty = 1);
	return (0);
}

int	game_local_select_nothing(struct s_game_local *game)
{
	if (!game)
		return (1);
	game->selected_item = SELECTED_ITEM_NONE;
	game->hand->card_selected[0] = -1;
	game->hand->card_selected[1] = -1;
	hand_update_selected(game->hand);
	if (game->swap_pile)
		pile_display_hide(game->swap_pile);
	if (game->swap_menu)
		menu_hide(game->swap_menu);
	return (0);
}

int game_local_select_action_menu(struct s_game_local *game)
{
	if (!game)
		return (1);
	if (game->player_action_menu)
	{
		game->selected_item = SELECTED_ITEM_ACTION_MENU;
		menu_show(game->player_action_menu);
	}
	return (0);
}

int game_local_select_hand(struct s_game_local *game)
{
	if (!game)
		return (1);
	if (game->hand)
	{
		game->selected_item = SELECTED_ITEM_HAND;
		hand_update_selected(game->hand);
		game->hand->hand_dirty = 1;
	}
	return (0);
}

int game_local_increment_whos_turn(struct s_game_local *game)
{
	if (!game)
		return (1);
	game->whos_turn = (game->whos_turn + 1) % (game->pdisplay_count + 1);
	return (0);
}

int game_local_who_won(struct s_game_local *game)
{
	int					who_won;

	if (!game)
		return (-1);
	if (game->play_state != PLAY_STATE_PLAY_PHASE)
		return (-1);
	who_won = -1;
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
	return (who_won);
}
