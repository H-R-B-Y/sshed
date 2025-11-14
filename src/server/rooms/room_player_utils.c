
# include "server/server.h"

int	game_room_add_player(struct s_game_room *room, struct s_connection *conn)
{
	struct s_player_data	*pdata;

	pdata = get_player_data_from_conn(conn);
	if (!pdata)
		return (-1); // No player data associated with connection
	if (room->player_count >= 4)
		return (-1); // Room full
	// Add player to room
	room->players[room->player_count] = conn;
	room->player_count++;
	// Update player data to reflect new session
	pdata->session = (void *)room;
	pdata->player_lobby_id = room->player_count - 1; // Assign lobby id
	return (0);
}

int	game_room_remove_player(struct s_game_room *room, struct s_connection *conn)
{
	struct s_player_data	*pdata;
	t_u8					i;
	t_u8					player_idx;

	pdata = get_player_data_from_conn(conn);
	if (!pdata)
		return (-1); // No player data associated with connection
	// Find player index in room
	player_idx = 255;
	for (i = 0; i < room->player_count; i++)
	{
		if (room->players[i] == conn)
		{
			player_idx = i;
			break;
		}
	}
	if (player_idx == 255)
		return (-1); // Player not found in room
	// Remove player from room by shifting others down
	for (i = player_idx; i < room->player_count - 1; i++)
	{
		room->players[i] = room->players[i + 1];
		// Update player_lobby_id for shifted players
		pdata = get_player_data_from_conn(room->players[i]);
		if (pdata)
			pdata->player_lobby_id = i;
	}
	room->players[room->player_count - 1] = NULL;
	room->player_count--;
	// Update player data to reflect removal from session
	pdata = get_player_data_from_conn(conn);
	if (pdata)
	{
		pdata->session = NULL;
		pdata->player_lobby_id = -1;
	}
	return (0);
}

