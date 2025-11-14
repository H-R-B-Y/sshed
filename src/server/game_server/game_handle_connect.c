# include "server/server.h"

int game_server_connect_handler(struct s_server *srv,
	struct s_connection *conn, void *appdata)
{
	// Appdata points to the gameserver struct
	struct s_game_server	*game_srv;

	game_srv = (struct s_game_server *)appdata;
	// When the client connects we dont need to do anything special because the first message they send
	// will be the header identifying who they are.
	(void)game_srv;
	(void)conn;
	return (1);
}
