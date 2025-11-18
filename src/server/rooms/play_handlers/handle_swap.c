
# include "server/server.h"

int	handle_player_action_swap(
	struct s_game_room *room,
	struct s_connection *conn,
	struct s_player_data *pdata,
	struct s_player_action_data *action
)
{
	switch (action->data_type)
	{

		case PLAYER_ACTION_DATA_SWAP_CARDS:
			/*
			Handle swapping cards
			*/
			// Validate the new shed face up cards
			// this means, check that the player has them all
			// and if they came from the hand
			// then swap them out
			for (t_u8 i = 0; i < 3; i++)
			{
				if (player_card_in_hand(pdata, action->data.swap_cards.shed_face_up[i]))
				{
					// swap from hand
					player_remove_card(pdata, action->data.swap_cards.shed_face_up[i]);
					// if there was a card already face up here, move it to hand
					if (pdata->shed_face_up_mask & (1 << i))
						player_add_card(pdata, pdata->shed_face_up[i]);
					// place new card face up
					pdata->shed_face_up[i] = action->data.swap_cards.shed_face_up[i];
					pdata->shed_face_up_mask |= (1 << i);
				}
				else
				{
					// invalid swap, player does not have the card in hand
					// TODO: send a error to the client
					send_error_to_client(
						conn,
						ERROR_INVALID_ACTION
					);
					return (1);
				}
			}
			break ;
		case PLAYER_ACTION_DATA_DEFAULT:
			break;
		case PLAYER_ACTION_DATA_PLAY_CARDS:
		default:
			send_error_to_client(
				conn,
				ERROR_INVALID_ACTION
			);
			return (1); // Unknown action type, ignore
	}
	room->swaps_done++;
	if (room->swaps_done >= room->player_count
		&& room->e_phase == PHASE_SWAP)
	{
		room_set_phase(room, PHASE_PLAY_CARDS);
		room_handle_phase(room);
	}
	increment_current_player(room);
	return (1);
}
