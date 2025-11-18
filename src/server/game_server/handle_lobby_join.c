#include "server/server.h"

int	handle_lobby_join(
	struct s_game_server *game_srv,
	struct s_connection *conn,
	struct s_player_data *pdata,
	struct s_message *msg
)
{
	struct s_game_room		*room;
	void					*data;
	struct s_header_chunk	header;
	struct s_game_room_join_request	*join_req;

	if (pdata->session != NULL)
		return (1);  // Already in session, ignore request
	if (msg->header->content_length != sizeof(struct s_game_room_join_request)
		|| !msg->content)
		return (1);  // Invalid content, ignore request
	join_req = (struct s_game_room_join_request *)msg->content;
	room = hm_get_value(
		&game_srv->active_rooms,
		&join_req->room_id
	);
	if (!room)
	{
		send_error_to_client(
				conn,
				ERROR_UNKNOWN
			);
		return (1);  // Room not found, ignore request
	}
	if (game_room_add_player(room, conn) != 0)
	{
		send_error_to_client(
				conn,
				ERROR_FAILED_TO_JOIN_ROOM
			);
		return (1);  // Failed to add player, ignore request
	}
	// Successfully added player to room
	header.content_length = sizeof(struct s_lobby_update);
	header.msg_type = MTYPE_LOBBY_UPDATE;
	data = create_lobby_update(LOBBY_PLAYER_JOINED, room->room_id,
			pdata->player_lobby_id, pdata->name);
	game_room_broadcast_message(room, &header, data, NULL);
	return (1);
}
