#ifndef GAME_STATE_DATA_H
# define GAME_STATE_DATA_H

# include "menu.h"

// Forward declarations
struct s_menu;

/**
 * @brief Game setup state data structure
 * 
 * Contains all the data needed for the local game setup state,
 * including player configuration and menu interface.
 */
struct s_game_local_setup
{
	/// @brief Number of players configured in the setup.
	unsigned int			player_count;
	/// @brief Some struct to contain the game custom rules TODO: define this properly
	void					*custom_rules;
	/// @brief The setup menu
	struct s_menu			*menu;
};

/**
 * @brief Settings menu state data structure
 * 
 * Contains all the data needed for the settings menu state.
 */
struct s_game_settings_menu
{
	/// @brief The settings menu data TODO: define properly
	void					*data;
	/// @brief The settings menu on the main menu
	struct s_menu			*menu;
};

#endif
