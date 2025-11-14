# include "server/server.h"

static unsigned int	hash_room_id(const void *key)
{
	t_u32	room_id;

	room_id = *(t_u32 *)key;
	return (room_id % 4093); // using a prime number for better distribution
}

static int	compare_room_id(const void *key1, const void *key2)
{
	t_u32	room_id1;
	t_u32	room_id2;

	room_id1 = *(t_u32 *)key1;
	room_id2 = *(t_u32 *)key2;
	if (room_id1 == room_id2)
		return (0);
	return (room_id1 < room_id2 ? -1 : 1);
}


struct s_game_server	*create_game_server(const char *socket_path)
{
	struct s_game_server	*srv;
	int						ev_fd;

	srv = ft_calloc(1, sizeof(struct s_game_server));
	if (!srv)
		return (NULL);
	if (server_init(&srv->base_server, socket_path) != 0)
	{
		free(srv);
		return (NULL);
	}
	srv->base_server.on_connect = game_server_connect_handler;
	srv->base_server.on_disconnect = game_server_disconnect_handler;
	srv->base_server.on_message = game_server_message_handler;
	srv->next_room_id = 1;
	srv->active_rooms = hm_create(hash_room_id, compare_room_id, 1024);
	srv->lobbies = cdll_init();
	srv->rooms_to_cleanup = cdll_init();
	ev_fd = eventfd(0, EFD_NONBLOCK | EFD_SEMAPHORE);
	if (ev_fd == -1)
	{
		free_game_server(srv);
		return (NULL);
	}
	if (register_aux_event(
			&srv->base_server,
			ev_fd,
			EPOLLIN,
			NULL,
			(void *)srv
		) != 0)
	{
		close(ev_fd);
		free_game_server(srv);
		return (NULL);
	}
	srv->room_cleanup_fd = ev_fd;
	return (srv);
}

void	free_game_server(struct s_game_server *srv)
{
	server_shutdown(&srv->base_server);
	hm_destroy(&srv->active_rooms);
	cdll_destroy(&srv->lobbies, free_game_room);
	cdll_destroy(&srv->rooms_to_cleanup, free_game_room);
	return ;
}
