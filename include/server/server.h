
#ifndef SERVER_H
# define SERVER_H

# include "shared.h"

// declare max cards here for player hands and decks
# define MAX_CARDS 54

// following should be moved to server only header

struct s_player_data
{
	// IP address of connection:
	t_u32				ip_address; // This is in case we do reconnections later
	void				*conn; // struct s_connection*
	void 				*session; // struct s_game_room*
	// -1 means not currently in a lobby
	t_s8				player_lobby_id; // ID of the player in this lobby (idx in the lobby players array)
	char				*name;
	/// @brief The cards in the players hand
	t_min_card_desc		hand[MAX_CARDS];
	/// @brief Number of cards in the players hand
	size_t				hand_count;
	t_min_card_desc		shed_face_up[3];
	t_u8				shed_face_up_mask; // bitmask for face up cards 101 means cards 0 and 2 are populated
	t_min_card_desc		shed_face_down[3];
	t_u8				shed_face_down_mask; // bitmask for face down cards
	t_u8				shed_card_count;
	t_u8				is_ready;
};

struct s_player_data	*create_player_data(char *name);
struct s_player_data	*get_player_data_from_conn(struct s_connection *conn);
void					free_player_data(struct s_player_data *pdata);

// Need some things to update the player state
int						player_add_card(struct s_player_data *pdata, t_min_card_desc card);
int						player_remove_card(struct s_player_data *pdata, t_min_card_desc card);

# ifndef SHED_CARD_MASKS
#  define SHED_CARD_MASKS
#  define SHED_CARD_0_MASK 0x01
#  define SHED_CARD_1_MASK 0x02
#  define SHED_CARD_2_MASK 0x04
# endif

int						player_add_card_faceup(struct s_player_data *pdata, t_min_card_desc card);
int						player_add_card_facedown(struct s_player_data *pdata, t_min_card_desc card);
int						player_remove_card_faceup(struct s_player_data *pdata, t_min_card_desc card);
int						player_remove_card_facedown(struct s_player_data *pdata, t_min_card_desc card);

int						player_has_card(struct s_player_data *pdata, t_min_card_desc card);
int						player_has_cards(struct s_player_data *pdata, t_min_card_desc *cards, size_t card_count);
int						player_card_in_hand(struct s_player_data *pdata, t_min_card_desc card);
int						player_card_shed_faceup(struct s_player_data *pdata, t_min_card_desc card);
int						player_card_shed_facedown(struct s_player_data *pdata, t_min_card_desc card);



// For updating THIS players client only (not broadcast)
/*
which user (lobby id)
how many cards in hand
array of card descs in hand
how many cards facedown in shed
how many cards faceup in shed
array of cards faceup in shed
*/
struct s_message		*create_player_update_message(
	struct s_game_room *room, struct s_player_data *pdata);

// For updating all players about THIS players hand count and shed cards
/*
which user this is about (lobby id)
how many cards in hand
how many cards facedown in shed
how many cards faceup in shed
array of cards faceup in shed
*/
struct s_message		*create_hand_info_message(
	struct s_game_room *room, struct s_player_data *pdata);

enum e_game_phase
{
		PHASE_NONE,
		PHASE_DEAL_CARDS,
		PHASE_SWAP,
		PHASE_PLAY_CARDS,
		PHASE_GAME_OVER
};

struct s_game_room
{
	t_u32					room_id;
	struct s_connection		*players[4];
	t_u8					player_count;
	t_u8					players_ready;
	t_u8					swaps_done;

	t_list					*log_entries; // list of struct s_game_log_entry*
	t_list					*chat;
	
	enum e_game_phase		e_phase;
	t_u8 					current_player_idx; // index in players array
	t_s8					who_won;

	struct s_deck			*deck;
	t_min_card_desc			play_pile[MAX_CARDS];
	size_t					play_pile_count;
	// lobby settings

	// other settings to come

};

int	room_set_phase(struct s_game_room *room, enum e_game_phase new_phase);
int	room_handle_phase(struct s_game_room *room);

// this will be called when the client wants to create a new lobby
struct s_game_room	*create_game_room(void);
void				free_game_room(struct s_game_room *room);

int					game_room_add_player(struct s_game_room *room, struct s_connection *conn);
int					game_room_remove_player(struct s_game_room *room, struct s_connection *conn);

int					game_room_broadcast_message(
	struct s_game_room *room,
	struct s_header_chunk *header,
	void *content,
	struct s_connection *exclude_conn);

int					game_room_send_to_player(
	struct s_game_room *room,
	struct s_header_chunk *header,
	void *content,
	struct s_connection *conn);

int					game_room_handle_player_message(struct s_game_room *room, struct s_connection *conn, struct s_message *msg);

int					game_room_send_state_to_player(struct s_game_room *room, struct s_connection *conn);
int					game_room_send_state_to_all_players(struct s_game_room *room);

int	increment_current_player(
	struct s_game_room *room
);

// deck state message for all players
/*
number of cards in deck
number of cards in play pile
array of cards in play pile
current phase
whoes turn it is (lobby id)
*/
struct s_message	*create_deck_state_message(struct s_game_room *room);

// game over message
/*
winner lobby id
*/
struct s_message	*create_game_over_message(struct s_game_room *room, t_s8 winner_lobby_id);


struct s_game_server
{
	struct s_server			base_server; // underlying sock server framework server

	t_u32					next_room_id;
	t_hashmap				active_rooms; // map of room id to struct s_game_room*
	t_cdll					lobbies; // list of struct s_game_room* 
	t_cdll					rooms_to_cleanup;
	int						room_cleanup_fd; // eventfd for server events
};

struct s_game_server	*create_game_server(const char *socket_path);
void					free_game_server(struct s_game_server *srv);

struct s_game_room	*game_server_add_new_room(struct s_game_server *game_srv);
int					game_server_remove_room(struct s_game_server *game_srv, t_u32 room_id);
int					game_server_add_room_to_cleanup(struct s_game_server *game_srv, struct s_game_room *room);

int game_server_connect_handler(struct s_server *srv,
	struct s_connection *conn, void *appdata);

int game_server_disconnect_handler(struct s_server *srv,
	struct s_connection *conn, void *appdata);

int game_server_message_handler(struct s_server *srv,
	struct s_message *message, void *appdata);


#endif
