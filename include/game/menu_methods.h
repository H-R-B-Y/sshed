/**
 * @file menu_methods.h
 * @brief Main menu state data structure
 * 
 * This header defines the data structure for the main menu game state.
 * State handler functions (load/unload) are declared in game_state_handlers.h
 */

#ifndef MENU_METHODS_H
# define MENU_METHODS_H

# include "menu.h"
# include <notcurses/notcurses.h>

/* ========================================================================== */
/*                         STATE DATA STRUCTURES                              */
/* ========================================================================== */

/**
 * @brief Main menu state data structure
 * 
 * Contains all the data needed for the main menu game state.
 * Currently only requires a menu interface.
 */
struct s_main_menu
{
	/// @brief The main menu interface
	struct s_menu			*menu;
};

#endif
