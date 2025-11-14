
#ifndef SHARED_H
# define SHARED_H

# include <stdarg.h>
# define FT_INCLUDE_ALL
# include "libft.h"
# include "cards.h"
# include "deck.h"
# include "sf_resources/include/shared_resource.h"
# include "sf_resources/include/sock_server_framework.h"

// we want to redefine a smaller type for card desc

/// @brief Minimal card description type (4 bits rank, 4 bits suit)
typedef t_u8 t_min_card_desc;

/**
 * @brief Encode a full card description into a minimal card description
 * 
 * @param card The card to encode
 * @return t_min_card_desc 
 */
static inline t_min_card_desc	encode_card_desc(struct s_card_desc card)
{
	return (
		((t_min_card_desc)(card.rank & 0x0F) << 4) |
		((t_min_card_desc)(card.suit & 0x0F))
	);
}

/**
 * @brief Decode a minimal card description into a full card description
 * 
 * @param min_card The min card to decode
 * @return struct s_card_desc 
 */
static inline struct s_card_desc	decode_card_desc(t_min_card_desc min_card)
{
	struct s_card_desc	card;

	card.rank = (t_rank)((min_card >> 4) & 0x0F);
	card.suit = (t_suit)(min_card & 0x0F);
	return (card);
}


enum e_message_types
{
	MTYPE_NONE,
	MTYPE_JOIN_HEADER, // client -> server: provides player name
	MTYPE_LOBBY_CREATE, // client -> server: create a new game lobby

	// client -> server: join an existing game lobby
	// server -> client: new player in lobby
	// this will tell the player what their lobby ID is
	MTYPE_LOBBY_JOIN,

	// client -> server: player leaving lobby or game
	// caused by disconnect or client leave
	// server -> client: player left lobby or game
	MTYPE_LOBBY_LEFT,

	// server -> client: current lobby state update
	// client -> server: updating lobby settings
	MTYPE_LOBBY_UPDATE,

	// client -> server: player is ready to start
	// server -> client: all players ready, game starting
	MTYPE_GAME_START,


	// server -> client: state of the deck and play pile and phase
	// and whos turn it is?
	MTYPE_DECK_STATE,

	// server -> client: how many cards / shed cards each player has
	MTYPE_HAND_INFO,

	// server -> client: your current hand of cards
	MTYPE_PLAYER_HAND,

	// client -> server : players action
	// we dont send this to other players, just send the resulitng state updates
	MTYPE_PLAYER_ACTION,


	// server -> client: game log entry
	MTYPE_GAME_LOG_ENTRY,

	// server -> client: game over, provides winner info
	MTYPE_GAME_OVER,

	MTYPE_WRONG, // server -> client: wrong action played

	MTYPE_ERROR_MSG, // server -> client: error message
	MTYPE_CHAT_MESSAGE, // client <-> server: chat message
	MTYPE_PING, // client <-> server: ping message for latency checks?

	MTYPE_TYPE_COUNT
};

/// @brief Lobby update types
enum e_lobby_update_type
{
	/// @brief Player joined the lobby
	LOBBY_PLAYER_JOINED,
	/// @brief Player left the lobby
	LOBBY_PLAYER_LEFT,
	/// @brief Lobby settings changed
	LOBBY_SETTINGS_CHANGED,
	/// @brief Player ready status changed
	LOBBY_PLAYER_READY_STATUS_CHANGED,
	/// @brief Lobby closed
	LOBBY_CLOSED,
	/// @brief Count of lobby update types
	LOBBY_UPDATE_TYPE_COUNT
};

/// @brief Lobby update structure
struct s_lobby_update
{
	/// @brief Type of update
	enum e_lobby_update_type	update_type;
	/// @brief Room ID
	t_u32		room_id;
	/// @brief Update data
	union e_lobby_update_data
	{
		/// @brief Player joined data
		struct s_player_joined
		{
			/// @brief Lobby ID of the player who joined
			t_u8	player_id;
			/// @brief Name of the player who joined
			char	player_name[32];
		}	player_joined;
		struct s_player_left
		{
			/// @brief Lobby ID of the player who left
			t_u8	player_id;
		}	player_left;
		struct s_settings_changed
		{
			/// @brief New starting hand size
			t_u8	starting_hand_size;
		}	settings_changed;
		struct s_player_ready_status_changed
		{
			/// @brief Lobby ID of the player
			t_u8	player_id;
			/// @brief New ready status
			t_u8	is_ready;
		}	player_ready_status_changed;
	}	data;
};

/**
 * @brief Create a lobby update object
 * 
 * LOBBY_PLAYER_JOINED: args: t_u8 player_id, char* player_name
 * LOBBY_PLAYER_LEFT: args: t_u8 player_id
 * LOBBY_SETTINGS_CHANGED: args: t_u8 starting_hand_size
 * LOBBY_PLAYER_READY_STATUS_CHANGED: args: t_u8 player_id, t_u8 is_ready
 * LOBBY_CLOSED: no args
 * 
 * @param update_type the update type
 * @param ... args for the update type
 * @return struct s_lobby_update* 
 */
struct s_lobby_update	*create_lobby_update(
	enum e_lobby_update_type update_type, t_u32 room, ...
);

/// @brief Structure of the game room join request
struct s_game_room_join_request
{
	t_u32	room_id;
};

enum e_player_action_data_type
{
	PLAYER_ACTION_DATA_DEFAULT,
	PLAYER_ACTION_DATA_PLAY_CARDS,
	PLAYER_ACTION_DATA_SWAP_CARDS,
	PLAYER_ACTION_DATA_TYPE_COUNT
};

/// @brief Data for a player action
struct s_player_action_data
{
	enum e_player_action_data_type	data_type;
	union u_player_action_data
	{
		struct s_play_cards_action
		{
			t_u8			card_count;
			t_min_card_desc	cards[4];
		}	play_cards;
		struct s_swap_cards_action
		{
			/*
			Since I dont want to deal with having to track where the cards are coming from,
			we will just write to the server what the new state of the players shed is after the swap
			and the server will validate that the player had those cards to swap.
			*/
			t_min_card_desc	shed_face_up[3];
		}	swap_cards;
	}	data;
};

/// @brief Structure of the chat message
/// @warning message length must not exceed 255 bytes + null terminator
struct s_player_chat
{
	t_u8		message_length;
	char	message[256];
};



#endif