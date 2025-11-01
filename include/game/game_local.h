
#ifndef GAME_LOCAL_H
# define GAME_LOCAL_H

# include "main_statemap.h"
# include "menu.h"
# include "deck.h"
# include "deck_display.h"
# include "hand.h"
# include "pdisplay.h"
# include "pile_display.h"

# include "player_action.h"
# include "ai_state_machine.h"

# include "enum_macro.h"

// Time out for the game internal logic to take a step through the state machine
# define FRAME_TIMEOUT 1

// Card per player
# define CARDS_PER_PLAYER (3 + 3 + 5) // 3 face down, 3 face up, 5 in hand

struct epoll_event;
struct s_game_manager;

# define GAME_PLAYING_STATES(prefix, func) \
	func(prefix, NONE) \
	func(prefix, DEAL_PHASE) \
	func(prefix, SWAP_PHASE) \
	func(prefix, PLAY_PHASE) \
	func(prefix, GAME_END) \
	func(prefix, ERROR)

ENUM_FACTORY(PLAY_STATE, game_play_state, GAME_PLAYING_STATES);
	

struct s_game_local_settings
{
	/// @brief Number of players in the game (1-3 AI)
	t_u8		player_count;
	/// @brief This should include more things like house rules, etc..
};

struct s_game_local
{
	/// @brief Whether the game has been initialised
	t_u8							initialised;
	/// @brief Game settings
	struct s_game_local_settings	settings;
	
	// Game components

	/// @brief The deck used in the game
	struct s_deck					*deck;
	/// @brief The deck display
	struct s_deck_display			*deck_display;
	/// @brief The player's hand
	struct s_hand					*hand;
	/// @brief The hands for each AI player
	struct s_pdisplay				*pdisplay[3];
	/// @brief The number of AI players in the game (kind of redundant with settings.player_count)
	t_u8							pdisplay_count;
	/// @brief The State data for each AI player
	struct s_ai_data				ai_data[3];
	/// @brief The pile display (play pile)
	struct s_pile_display			*pile_display;
	/// @brief Current play state
	t_game_play_state				play_state;
	/// @brief Number of cards played in totoal this game (TODO: this should be built into a stats struct)
	int								cards_played;
	/// @brief Whos turn it is (0 = player, 1-3 = AI)
	t_u8							whos_turn;
	/// @brief The action struct representing the current player's action
	struct s_player_action			player_action;

	/// @brief Who won the game (0 = player, 1-3 = AI)
	/// @note This needs to be passed to the end game state, maybe we should have some better mechanism for retrieving this info
	t_u8							who_won;

	/// @brief Linked list of log lines (TODO: figure out the log)
	t_list							*game_log; 
	/// @brief Visual log plane
	struct ncplane					*log_plane;
	/// @brief Maximum number of log lines to keep
	unsigned int					max_log_line;
	/// @brief Whether the log is visible
	t_u8							log_visible;
	/// @brief Whether the log needs to be re-rendered
	t_u8							log_dirty;
};

/**
 * @brief Load function for the local game state
 * 
 * This function will check what the previous state was:
 * If the previous state was a paused local game, it will restore that state.
 * If the previous state was a game_local_setup state, it will load the settings from there.
 * If there is no previous state, it will load default settings.
 * 
 * @param manager The game manager
 * @param state_data Pointer to store the loaded state data
 * @return 0 on success, 1 on error
 */
int		load_game_local(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload function for the local game state
 * @param manager The game manager
 * @param state_data The state data to unload
 */
void	unload_game_local(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Free function for the local game state
 * @param game The game local state to free
 */
void	free_game_state(
	struct s_game_local *game
);

/**
 * @brief Render loop callback to update the game
 * 
 * This function handles the game logic updates, like performing the actions of players.
 * This will also check for win conditions, deal cards, and manage the game state transitions.
 * 
 * @param manager The game manager
 * @return 0 on success, 1 on error
 */
int		pre_render_game_update(
	struct s_game_manager *manager
);

/**
 * @brief Input handler for the local game play state
 * 
 * This function handles input events during the local game play state.
 * It processes player actions based on the input received.
 * 
 * TODO: Since we want to move to a better input model where we can select
 * different UI elements, this function will need to be updated to handle
 * those interactions properly.
 * 
 * @param manager The game manager
 * @param event The epoll event containing input data
 * @return 0 on success, 1 on error
 */
int		game_local_play_input_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
);

struct s_game_local_pause
{
	/// @brief The pause menu
	struct s_menu		*menu;
};

/**
 * @brief Load function for the local game pause state
 * 
 * @param manager The game manager
 * @param state_data Pointer to store the loaded state data
 * @return 0 on success, 1 on error
 */
int		load_local_pause(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload function for the local game pause state
 * @param manager The game manager
 * @param state_data The state data to unload
 */
void	unload_local_pause(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Free function for the local game pause state
 * @param pause The pause state to free
 */
void	free_local_pause(
	struct s_game_local_pause *pause
);


struct s_game_local_end
{
	/// @brief Who won the game (0 = player, 1-3 = AI)
	t_u8					who_won;
	/// @brief Linked list of log lines
	t_list					*log_data;
	/// @brief Visual log plane
	struct ncplane			*log_plane;
	/// @brief Whether the log is visible
	t_u8					log_visible;
	/// @brief Whether the log needs to be re-rendered
	t_u8					log_dirty;
	/// @brief The end game menu
	struct s_menu			*menu;
};

/**
 * @brief Load function for the local game end state
 * 
 * This function expects to be able to retrieve the winner information
 * from the previous state data, then free's the prev data.
 * 
 * If no previous state data is available, it will default to
 * the player winning.
 * 
 * @param manager The game manager
 * @param state_data Pointer to store the loaded state data
 * @return 0 on success, 1 on error
 */
int		load_local_end(
	struct s_game_manager *manager,
	void **state_data
);

/**
 * @brief Unload function for the local game end state
 * @param manager The game manager
 * @param state_data The state data to unload
 */
void	unload_local_end(
	struct s_game_manager *manager,
	void *state_data
);

/**
 * @brief Free function for the local game end state
 * @param end_state The end state to free
 */
void	free_local_end(
	struct s_game_local_end *end_state
);


#endif
