#include "server/server.h"

/*
** Handles lobby/room setup messages BEFORE the game starts
** This includes creating rooms, joining, leaving, updating settings, and starting
** Once the game starts (PHASE != PHASE_NONE), messages route to room_handle_player_message()
*/
int handle_game_room_message(
	struct s_game_server *game_srv,
	struct s_connection *conn,
	struct s_message *msg
)
{
	struct s_player_data	*pdata;

	pdata = get_player_data_from_conn(conn);
	if (!pdata)
	{
		send_error_to_client(
				conn,
				ERROR_INVALID_MESSAGE
			);
		return (0);  // No player data, disconnect
	}
	switch ((enum e_message_types)msg->header->msg_type)
	{
		case MTYPE_LOBBY_CREATE:
			return handle_lobby_create(game_srv, conn, pdata);
		case MTYPE_LOBBY_JOIN:
			return handle_lobby_join(game_srv, conn, pdata, msg);
		case MTYPE_LOBBY_UPDATE:
			// TODO: extract data from message to see what to update
			// TODO: Validate player is in a room and game hasn't started
			// TODO: Apply settings changes
			// TODO: Broadcast updated settings to all players
			return (1);
		case MTYPE_GAME_START:
			return handle_game_start(game_srv, conn, pdata);
		default:
			send_error_to_client(
				conn,
				ERROR_INVALID_MESSAGE
			);
			return (1);  // Ignore unknown message
	}
}
