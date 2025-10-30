
#ifndef MAIN_STATEMAP_H
# define MAIN_STATEMAP_H

// So we want to make a state map for the main game states

# include "../enum_macro.h"

# define GAME_STATE_LIST(prefix, func) \
	func(prefix, NONE) \
	func(prefix, INIT) \
	func(prefix, MAIN) \
	func(prefix, SETTINGS) \
	func(prefix, GAME_LOCAL_SETUP) \
	func(prefix, GAME_LOCAL_PLAY) \
	func(prefix, GAME_LOCAL_PAUSE) \
	func(prefix, GAME_LOCAL_END) \
	func(prefix, GAME_NEW_STATE)

ENUM_FACTORY(GAME_STATE, game_state, GAME_STATE_LIST);


#endif
