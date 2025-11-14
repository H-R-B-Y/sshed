#include "server/server.h"

/*
** Main message handler - routes incoming messages to appropriate handlers
** This is the entry point called by sf_resources server framework
*/
int game_room_message_handler(
	struct s_server *srv,
	struct s_message *message,
	void *appdata
)
{
	struct s_game_server	*game_srv;
	struct s_player_data	*pdata;

	game_srv = (struct s_game_server *)appdata;
	
	// First message MUST be JOIN_HEADER
	if (!message->sender->user_data)
	{
		if (message->header->msg_type == MTYPE_JOIN_HEADER)
			return handle_player_header_message(game_srv, message->sender, message);
		// Wrong first message - disconnect
		// TODO: Send MTYPE_ERROR_MSG before disconnecting
		return (0);  // Disconnect
	}
	
	// Now we have player data, route to appropriate handler
	pdata = (struct s_player_data *)message->sender->user_data;
	
	// Route to lobby handler (pre-game) or in-game handler
	switch ((enum e_message_types)message->header->msg_type)
	{
		case MTYPE_JOIN_HEADER:
			// Already have player data, should not receive this again
			return (0);  // Disconnect (security concern)
		
		case MTYPE_LOBBY_LEFT:
			// Player leaving room - can happen any time (pre-game or in-game)
			return handle_lobby_left(game_srv, message->sender, pdata);
		
		case MTYPE_LOBBY_CREATE:
		case MTYPE_LOBBY_JOIN:
		case MTYPE_LOBBY_UPDATE:
		case MTYPE_GAME_START:
			// Handle lobby/room management messages (PRE-GAME only)
			return handle_game_room_message(game_srv, message->sender, message);
		
		case MTYPE_PLAYER_ACTION:
		case MTYPE_CHAT_MESSAGE:
			// Only valid if in a game room
			if (!pdata->session)
				return (1);  // Not in game, ignore
			// Route to in-game message handler
			return room_handle_player_message(
				(struct s_game_room *)pdata->session,
				message->sender,
				message);
		
		case MTYPE_PING:
			// TODO: Send MTYPE_PING back to client
			return (1);
		
		// These are server->client only, client should never send these
		// not entirely true, clients could send these as a request for the state
		// maybe implement that later
		case MTYPE_DECK_STATE:
		case MTYPE_HAND_INFO:
		case MTYPE_PLAYER_HAND:
		case MTYPE_GAME_OVER:
		case MTYPE_WRONG:
		case MTYPE_ERROR_MSG:
			// Invalid message direction, disconnect
			return (0);
		
		default:
			// Unknown message type, ignore
			return (1);
	}
}
