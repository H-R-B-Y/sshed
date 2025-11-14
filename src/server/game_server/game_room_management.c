
# include "server/server.h"

int	cleanup_rooms_callback(
	struct s_server *srv,
	struct epoll_event *event_data,
	void *appdata
)
{
	// appdata is struct s_game_server*
	struct s_game_server	*game_srv;
	struct s_cdll_node		*node;
	struct s_game_room		*room;
	ssize_t					ret;
	uint64_t				value;

	ret = eventfd_read(event_data->data.fd, &value);
	if (ret == -1)
		return (-1); // Failed to read eventfd
	game_srv = (struct s_game_server *)appdata;
	// Now we cleanup a single room from the rooms_to_cleanup list
	if (game_srv->rooms_to_cleanup.count == 0)
		return (0); // Nothing to cleanup
	node = cdll_pop_front(&game_srv->rooms_to_cleanup);
	room = node->data;
	free(node);
	free_game_room(room);
	return (0);
}

int	game_server_add_room_to_cleanup(
	struct s_game_server *game_srv,
	struct s_game_room *room
)
{
	struct s_cdll_node	*node;

	if (!game_srv || !room)
		return (-1);
	node = ft_calloc(1, sizeof(struct s_cdll_node));
	if (!node)
		return (-1);
	node->data = (void *)room;
	if (room->player_count)
	{
		// send them all a disconnect message
		for (int i = 0; i < room->player_count; i++)
			game_room_remove_player(room, room->players[0]);
	}
	cdll_push_back(&game_srv->rooms_to_cleanup, node);
	eventfd_write(
		game_srv->room_cleanup_fd,
		1
	);
	return (0);
}

int	game_server_remove_room(
	struct s_game_server *game_srv,
	t_u32 room_id
)
{
	struct s_game_room	*room;

	if (!game_srv)
		return (-1);
	room = hm_get_value(
		&game_srv->active_rooms,
		&room_id
	);
	if (!room)
		return (-1); // Room not found
	hm_remove_pair(
		&game_srv->active_rooms,
		&room_id
	);
	// Add room to cleanup list
	return (game_server_add_room_to_cleanup(game_srv, room));
}

struct s_game_room	*game_server_add_new_room(
	struct s_game_server *game_srv
)
{
	struct s_game_room	*room;
	t_u32				room_id;

	if (!game_srv)
		return (NULL);
	room = ft_calloc(1, sizeof(struct s_game_room));
	if (!room)
		return (NULL);
	room_id = game_srv->next_room_id++;
	room->room_id = room_id;
	room->e_phase = PHASE_NONE;
	room->deck = NULL;
	room->play_pile_count = 0;
	room->log_entries = NULL;
	room->chat = NULL;
	hm_add_value(
		&game_srv->active_rooms,
		&room_id,
		room
	);
	cdll_push_back(&game_srv->lobbies, ft_calloc(1, sizeof(struct s_cdll_node)));
	game_srv->lobbies.tail->data = (void *)room;
	return (room);
}
