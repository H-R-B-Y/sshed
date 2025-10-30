
# include "game/game_data.h"
# include "game/game_local.h"

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
	else if (c_code == NCKEY_ENTER
		|| c_code == '\n'
		|| c_code == '\r'
		|| c_code == NCKEY_SPACE
	)
	{
		if (game->whos_turn == 0 && !game->player_action.ready
			&& (game->hand->card_count > 0 || game->hand->shed_count > 0)
			&& game->hand->selected_card_plane)
		{
			// Make sure that player can only play cards from the hand
			// when there are cards in the hand
			// and that displaying the shed does not make the cards draw from the shed
			// TODO: add better logic here
			struct s_card_desc	card;
			card = game->hand->selected_card_plane->card_desc;
			if (
				(game->hand->status == HAND_DISPLAY_HAND
				&& game->hand->card_count > 0
				&& !hand_card_in_shed(game->hand, card))
				||
				(game->hand->status == HAND_DISPLAY_SHED
				&& game->hand->card_count == 0
				&& hand_card_in_shed(game->hand, card))
			)
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
	else if (c_code == NCKEY_BACKSPACE)
	{
		notcurses_refresh(manager->nc, NULL, NULL);
	}
	return (0);
}
