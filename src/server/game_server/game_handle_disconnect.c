# include "server/server.h"

int	game_server_disconnect_handler(struct s_server *srv,
	struct s_connection *conn, void *appdata)
{
	struct s_game_server *game_srv = (struct s_game_server *)appdata;
	struct s_player_data *pdata;
	struct s_game_room *room;
	t_u8 player_id;
	int game_in_progress;
	
	if (!conn->user_data)
		return (0);  // No player data, nothing to clean up
	
	pdata = (struct s_player_data *)conn->user_data;
	
	// If player is in a room, handle removal
	if (pdata->session)
	{
		room = (struct s_game_room *)pdata->session;
		player_id = pdata->player_lobby_id;
		game_in_progress = (room->e_phase != PHASE_NONE && 
			room->e_phase != PHASE_GAME_OVER);
		
		// If game is in progress, shutdown the game
		if (game_in_progress)
		{
			// Broadcast lobby closed to all players
			void *data = create_lobby_update(LOBBY_CLOSED, room->room_id);
			struct s_header_chunk header;
			header.msg_type = MTYPE_LOBBY_UPDATE;
			header.content_length = sizeof(struct s_lobby_update);
			game_room_broadcast_message(room, &header, data, NULL);
			free(data);
			
			// Remove all players from the room
			while (room->player_count > 0)
			{
				game_room_remove_player(room, room->players[0]);
			}
			
			// Queue room for cleanup
			game_server_add_room_to_cleanup(game_srv, room);
		}
		else
		{
			// Not in game, just remove this player
			game_room_remove_player(room, conn);
			
			// Broadcast player left message
			void *data = create_lobby_update(LOBBY_PLAYER_LEFT, 
				room->room_id, player_id);
			struct s_header_chunk header;
			header.msg_type = MTYPE_LOBBY_UPDATE;
			header.content_length = sizeof(struct s_lobby_update);
			game_room_broadcast_message(room, &header, data, NULL);
			free(data);
			
			// If room is empty, clean it up
			if (room->player_count == 0)
			{
				game_server_add_room_to_cleanup(game_srv, room);
			}
		}
	}
	
	// player_data will be freed by cleanup_user_data callback
	return (0);
}
