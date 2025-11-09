/**
 * @file game_state_data.h
 * @brief Data structures for various game states
 * 
 * This header defines the data structures used by different game states.
 * Each structure contains all state-specific data needed during that
 * particular game state. State handler functions (load/unload) are
 * declared in game_state_handlers.h
 * 
 * States defined here:
 * - Game setup state (player count, custom rules)
 * - Settings menu state
 */

#ifndef GAME_STATE_DATA_H
# define GAME_STATE_DATA_H

# include "menu.h"

/* ========================================================================== */
/*                         STATE DATA STRUCTURES                              */
/* ========================================================================== */

/**
 * @brief Game setup state data structure
 * 
 * Contains all the data needed for the local game setup state,
 * including player configuration and menu interface.
 */
struct s_game_local_setup
{
	/// @brief Number of players configured in the setup (1-4)
	unsigned int			player_count;
	/// @brief Custom game rules configuration
	/// @todo Define this structure properly
	void					*custom_rules;
	/// @brief The setup menu interface
	struct s_menu			*menu;
};

/**
 * @brief Settings menu state data structure
 * 
 * Contains all the data needed for the settings menu state.
 */
struct s_game_settings_menu
{
	/// @brief The settings menu data
	/// @todo Define this structure properly
	void					*data;
	/// @brief The settings menu interface
	struct s_menu			*menu;
};

#endif
