#include "server/server.h"

int	handle_lobby_create(
	struct s_game_server *game_srv,
	struct s_connection *conn,
	struct s_player_data *pdata
)
{
	struct s_game_room		*room;
	void					*data;
	struct s_header_chunk	header;

	if (pdata->session != NULL)
	{
		send_error_to_client(
				conn,
				ERROR_INVALID_MESSAGE
			);
		return (1);  // Already in session, ignore request
	}
	room = game_server_add_new_room(game_srv);
	if (!room)
	{
		send_error_to_client(
				conn,
				ERROR_FAILED_TO_CREATE_ROOM
			);
		return (1);  // Failed to create room, but don't disconnect
	}
	if (game_room_add_player(room, conn) != 0)
	{
		send_error_to_client(
				conn,
				ERROR_FAILED_TO_JOIN_ROOM
			);
		// Failed to add player to room
		game_server_add_room_to_cleanup(game_srv, room);
		return (1);  // Failed but don't disconnect
	}
	// Successfully created room and added player
	header.content_length = sizeof(struct s_lobby_update);
	header.msg_type = MTYPE_LOBBY_UPDATE;
	data = create_lobby_update(LOBBY_PLAYER_JOINED, room->room_id,
			pdata->player_lobby_id, pdata->name);
	game_room_broadcast_message(room, &header, data, NULL);
	return (1);
}
