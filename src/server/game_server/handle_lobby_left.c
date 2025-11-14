#include "server/server.h"

int	handle_lobby_left(
	struct s_game_server *game_srv,
	struct s_connection *conn,
	struct s_player_data *pdata
)
{
	struct s_game_room		*room;
	t_u8					player;
	void					*data;
	struct s_header_chunk	header;

	if (pdata->session == NULL)
		return (1);  // Not in session, ignore request
	player = pdata->player_lobby_id;
	room = (struct s_game_room *)pdata->session;

	// Need to figure out how to handle this event when the game is in progress vs
	// when it's still in the lobby phase.
	if (room->e_phase != PHASE_NONE)
	{
		// TODO: maybe create an error state
		// maybe update the lobby in some way to indicate the player left mid game
		// like move the players cards to the pickup pile and shuffle.
		// For now just remove them from the room and we can handle it later.
		;
	}

	if (game_room_remove_player(room, conn) != 0)
		return (1);  // Failed to remove, ignore request
	// Successfully removed player from room
	header.content_length = sizeof(struct s_lobby_update);
	header.msg_type = MTYPE_LOBBY_UPDATE;
	data = create_lobby_update(LOBBY_PLAYER_LEFT, room->room_id, player);
	game_room_broadcast_message(room, &header, data, NULL);
	// Check if room is now empty
	if (room->player_count == 0)
		game_server_add_room_to_cleanup(game_srv, room);
	return (1);
}
