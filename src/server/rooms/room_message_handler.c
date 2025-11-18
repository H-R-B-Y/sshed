
#include "server/server.h"

/*
** Handles messages from players during an active game
** This is called ONLY when the game has started (room->e_phase != PHASE_NONE)
** Pre-game lobby messages are handled by handle_game_room_message()
*/
int	room_handle_player_message(
	struct s_game_room *room,
	struct s_connection *conn,
	struct s_message *msg
)
{
	struct s_player_data	*pdata;

	// Validate player is in this room
	pdata = get_player_data_from_conn(conn);
	if (!pdata || pdata->session != room)
		return (0);  // Player not in this room, disconnect

	// Only handle messages if game has started
	if (room->e_phase == PHASE_NONE)
		return (1);  // Game hasn't started yet, ignore

	switch ((enum e_message_types)msg->header->msg_type)
	{
		case MTYPE_PLAYER_ACTION:
			// TODO: Validate it's this player's turn
			if (room->current_player_idx != pdata->player_lobby_id)
			{
				send_error_to_client(
					conn,
					ERROR_NOT_YOUR_TURN
				);
				return (1); // Not an error to send an action, just ignore it.
			}
			switch (room->e_phase)
			{
				case (PHASE_NONE):
				case (PHASE_DEAL_CARDS):
					send_error_to_client(
						conn,
						ERROR_NOT_YOUR_TURN
					);
					return (1); // Ignore action
				case (PHASE_SWAP):
					// TODO: do we want to do some prevalidation to make sure the data is the right size
					// and make sure that the data exists, probably
					return (handle_player_action_swap(room, conn, pdata, msg->content));
				case (PHASE_PLAY_CARDS):
					// TODO: see above
					return (handle_player_action_play(room, conn, pdata, msg->content));
				default :
					game_room_send_state_to_all_players(room);
					return (1); // We might want to flush the state to the clients?
			}
			return (1);

		case MTYPE_CHAT_MESSAGE:
			// TODO: Validate and sanitize chat message
			// TODO: Broadcast to all players in room
			return (1);

		default:
			send_error_to_client(
				conn,
				ERROR_INVALID_MESSAGE
			);
			return (1);
	}
}


