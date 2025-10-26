
#ifndef MAIN_STATEMAP_H
# define MAIN_STATEMAP_H

// So we want to make a state map for the main game states

# include "../enum_macro.h"

# define GAME_STATE_LIST(prefix, func) \
	func(prefix, INIT) \
	func(prefix, MAIN) \
	func(prefix, GAME_SETTINGS) \
	func(prefix, GAME_PLAY) \
	func(prefix, GAME_PAUSE) \
	func(prefix, GAME_END)

ENUM_FACTORY(GAME_STATE, game_state, GAME_STATE_LIST);

/*
typedef enum e_game_state
{
GAME_STATE_INIT,
GAME_STATE_MAIN,
GAME_STATE_GAME_SETTINGS,
GAME_STATE_GAME_PLAY,
GAME_STATE_GAME_PAUSE,
GAME_STATE_GAME_END,
GAME_STATE_COUNT
} t_game_state;
static const char *game_state_str[GAME_STATE_COUNT] __attribute__((unused))
 = { "INIT", "MAIN", "GAME_SETTINGS", "GAME_PLAY", "GAME_PAUSE", "GAME_END", };

*/

#endif
