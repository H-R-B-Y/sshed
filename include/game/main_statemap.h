/**
 * @file main_statemap.h
 * @brief Main game state enumeration
 * 
 * This header defines the primary game state machine enum.
 * Each value represents a distinct state that the game can be in,
 * with corresponding load/unload handlers defined in game_state_handlers.h
 * 
 * State flow:
 * NONE -> INIT -> MAIN -> (SETTINGS | GAME_LOCAL_SETUP -> GAME_LOCAL_PLAY -> 
 *                          GAME_LOCAL_PAUSE | GAME_LOCAL_END)
 */

#ifndef MAIN_STATEMAP_H
# define MAIN_STATEMAP_H

# include "../enum_macro.h"

/* ========================================================================== */
/*                         GAME STATE ENUMERATION                             */
/* ========================================================================== */

/**
 * @brief Game state list macro
 * 
 * Defines all possible game states:
 * - NONE: No state (default/uninitialized)
 * - INIT: Initialization state
 * - MAIN: Main menu
 * - SETTINGS: Settings menu
 * - GAME_LOCAL_SETUP: Local game setup
 * - GAME_LOCAL_PLAY: Local game playing
 * - GAME_LOCAL_PAUSE: Local game paused
 * - GAME_LOCAL_END: Local game ended
 * - GAME_NEW_STATE: Placeholder for future states
 */
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
