
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
			// TODO: Validate the action is legal
			// TODO: Apply the action to game state
			// TODO: Broadcast state updates to all players
			// TODO: Advance to next player or phase
			return (1);

		case MTYPE_CHAT_MESSAGE:
			// TODO: Validate and sanitize chat message
			// TODO: Broadcast to all players in room
			return (1);

		default:
			// Message not handled here, ignore
			return (1);
	}
}


