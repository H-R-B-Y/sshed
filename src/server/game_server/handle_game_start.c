#include "server/server.h"

static void	broadcast_ready_status(
	struct s_game_room *room,
	struct s_player_data *pdata,
	int ready_status
)
{
	void					*data;
	struct s_header_chunk	header;

	data = create_lobby_update(LOBBY_PLAYER_READY_STATUS_CHANGED,
			room->room_id, pdata->player_lobby_id, ready_status);
	header.content_length = sizeof(struct s_lobby_update);
	header.msg_type = MTYPE_LOBBY_UPDATE;
	game_room_broadcast_message(room, &header, data, NULL);
}

static void	start_game(struct s_game_room *room)
{
	void					*data;
	struct s_header_chunk	header;

	// Broadcast game start message
	data = NULL;
	header.content_length = 0;
	header.msg_type = MTYPE_GAME_START;
	game_room_broadcast_message(room, &header, data, NULL);
	// Change phase and initialize game
	room->e_phase = PHASE_DEAL_CARDS;
	// TODO: Call game_room_deal_cards(room)
	// TODO: Broadcast initial game state to all players
	game_room_send_state_to_all_players(room);
}

int	handle_game_start(
	struct s_game_server *game_srv,
	struct s_connection *conn,
	struct s_player_data *pdata
)
{
	struct s_game_room		*room;

	room = (struct s_game_room *)pdata->session;
	if (!room)
		return (1);  // Not in a room, ignore
	// Toggle ready state
	if (pdata->is_ready)
	{
		pdata->is_ready = 0;
		room->players_ready--;
		broadcast_ready_status(room, pdata, 0);
	}
	else
	{
		pdata->is_ready = 1;
		room->players_ready++;
		broadcast_ready_status(room, pdata, 1);
	}
	// Check if all players are ready
	if (room->players_ready == room->player_count && room->player_count >= 2)
		start_game(room);
	return (1);
}
