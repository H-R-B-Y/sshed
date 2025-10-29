
#ifndef GAME_LOCAL_H
# define GAME_LOCAL_H

# include "main_statemap.h"
# include "menu.h"
# include "deck.h"
# include "deck_display.h"
# include "hand.h"
# include "pdisplay.h"
# include "pile_display.h"

# include "ai_state_machine.h"

# include "enum_macro.h"

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
	t_u8		player_count;
	// some kind of rules or something
};

struct s_game_local
{
	t_u8							initialised;
	struct s_game_local_settings	settings;
	
	struct s_deck					*deck;
	struct s_deck_display			*deck_display;
	struct s_hand					*hand;
	struct s_pdisplay				*pdisplay[3];
	struct s_ai_data				ai_data[3];
	t_u8							pdisplay_count;
	struct s_pile_display			*pile_display;

	t_game_play_state				play_state;
	int								cards_played;

	t_u8							whos_turn;
	t_u8							card_has_been_played;
	t_card_desc						card_played;

	t_u8							who_won;

	// And we need some way to store the game log
	t_list							*game_log; // list of status messages
	struct ncplane					*log_plane;
	unsigned int					max_log_line;
	t_u8							log_visible;
	t_u8							log_dirty;
};

int		load_game_local(struct s_game_manager *manager, void **state_data);
void	unload_game_local(struct s_game_manager *manager, void *state_data);
void	free_game_state(struct s_game_local *game);

int		pre_render_game_update(struct s_game_manager *manager);

struct epoll_event;
int		game_local_play_input_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
);

struct s_game_local_pause
{
	// We might be able to get away with using this also for the multiplayer game but not sure how we are implemeting the multiplayer yet so we will have to see
	struct s_menu		*menu;
};

int		load_local_pause(struct s_game_manager *manager, void **state_data);
void	unload_local_pause(struct s_game_manager *manager, void *state_data);
void	free_local_pause(struct s_game_local_pause *pause);


struct s_game_local_end
{
	t_u8					who_won;
	t_list					*log_data;
	struct ncplane			*log_plane;
	struct s_menu			*menu;
};

int		load_local_end(struct s_game_manager *manager, void **state_data);
void	unload_local_end(struct s_game_manager *manager, void *state_data);
void	free_local_end(struct s_game_local_end *end_state);


#endif
