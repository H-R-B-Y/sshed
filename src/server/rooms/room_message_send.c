
# include "server/server.h"

int					game_room_broadcast_message(
	struct s_game_room *room,
	struct s_header_chunk *header,
	void *content,
	struct s_connection *exclude_conn)
{
	t_u8 				i;

	for (i = 0; i < room->player_count; i++)
	{
		if (room->players[i] != exclude_conn)
		{
			send_to_connection_ref(
				NULL,
				room->players[i],
				header,
				content
			);
		}
	}
	return (0);
}

int					game_room_send_to_player(
	struct s_game_room *room,
	struct s_header_chunk *header,
	void *content,
	struct s_connection *conn)
{
	t_u8 				i;

	for (i = 0; i < room->player_count; i++)
	{
		if (room->players[i] == conn)
		{
			send_to_connection_ref(
				NULL,
				conn,
				header,
				content
			);
			return (0);
		}
	}
	return (-1); // Player not found in room
}

int	game_room_send_state_to_player(
	struct s_game_room *room,
	struct s_connection *conn
)
{
	struct s_message		*msg;
	struct s_player_data	*pdata;

	pdata = get_player_data_from_conn(conn);
	if (!pdata || pdata->session != room)
		return (-1);  // Player not in this room
	// Ok so state is: (about all players)
	// hand info message
	for (t_u8 i = 0; i < room->player_count; i++)
	{
		pdata = get_player_data_from_conn(room->players[i]);
		if (!pdata)
			continue ;
		msg = create_hand_info_message(room, pdata);
		if (!msg)
			continue ;
		game_room_send_to_player(room, msg->header, msg->content, conn);
		// Free message
		if (msg->header)
			free(msg->header);
		if (msg->content)
			free(msg->content);
		free(msg);
	}
	// player hand message
	pdata = get_player_data_from_conn(conn);
	if (!pdata)
		return (-1);
	msg = create_player_update_message(room, pdata);
	if (!msg)
		return (-1);
	game_room_send_to_player(room, msg->header, msg->content, conn);
	// Free message
	if (msg->header)
		free(msg->header);
	if (msg->content)
		free(msg->content);
	free(msg);
	// deck state message
	msg = create_deck_state_message(room);
	if (!msg)
		return (-1);
	game_room_send_to_player(room, msg->header, msg->content, conn);
	// Free message
	if (msg->header)
		free(msg->header);
	if (msg->content)
		free(msg->content);
	free(msg);
	return (0);
}

int	game_room_send_state_to_all_players(struct s_game_room *room)
{
	struct s_message	*hand_message[4];
	struct s_message	*player_message[4];
	struct s_message	*deck_message;
	t_u8				i;
	struct s_player_data	*pdata[4];

	// Prepare messages
	for (i = 0; i < room->player_count; i++)
	{
		pdata[i] = get_player_data_from_conn(room->players[i]);
		if (!pdata[i])
			return (-1);
		hand_message[i] = create_hand_info_message(room, pdata[i]);
		if (!hand_message[i])
			return (-1);
		player_message[i] = create_player_update_message(room, pdata[i]);
		if (!player_message[i])
			return (-1);
	}
	deck_message = create_deck_state_message(room);
	if (!deck_message)
		return (-1);
	// Send messages
	for (i = 0; i < room->player_count; i++)
	{
		// Send hand info messages about all players
		for (t_u8 j = 0; j < room->player_count; j++)
			game_room_send_to_player(room, hand_message[j]->header, hand_message[j]->content, room->players[i]);
		// Send player hand message to each player
		game_room_send_to_player(room, player_message[i]->header, player_message[i]->content, room->players[i]);
		// Send deck state message to each player
		game_room_send_to_player(room, deck_message->header, deck_message->content, room->players[i]);
	}
	// Free messages
	for (i = 0; i < room->player_count; i++)
	{
		if (hand_message[i])
		{
			if (hand_message[i]->header)
				free(hand_message[i]->header);
			if (hand_message[i]->content)
				free(hand_message[i]->content);
			free(hand_message[i]);
		}
		if (player_message[i])
		{
			if (player_message[i]->header)
				free(player_message[i]->header);
			if (player_message[i]->content)
				free(player_message[i]->content);
			free(player_message[i]);
		}
	}
	if (deck_message)
	{
		if (deck_message->header)
			free(deck_message->header);
		if (deck_message->content)
			free(deck_message->content);
		free(deck_message);
	}
	return (0);
}
