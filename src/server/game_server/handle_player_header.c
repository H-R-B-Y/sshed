#include "server/server.h"

int	handle_player_header_message(
	struct s_game_server *game_srv,
	struct s_connection *conn,
	struct s_message *msg
)
{
	struct s_player_data	*pdata;

	// Validate message type
	if (msg->header->msg_type != MTYPE_JOIN_HEADER)
		return (0);  // Wrong message, disconnect
	// Validate content
	if (msg->header->content_length == 0 || !msg->content)
		return (0);  // Invalid content, disconnect
	// Check if player already initialized
	if (conn->user_data != NULL)
		return (0);  // Already has data, disconnect (security concern)
	// Create player data
	pdata = create_player_data(msg->content);
	if (!pdata)
		return (0);  // Memory allocation failed, disconnect
	// Initialize player connection data
	pdata->conn = (void *)conn;
	pdata->ip_address = 0; // TODO: extract from conn struct later
	conn->user_data = (void *)pdata;
	conn->cleanup_user_data = (t_freefn)free_player_data;
	// TODO: Send welcome message to client
	return (1);  // Success
}
