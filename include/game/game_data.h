
#ifndef GAME_DATA_H
# define GAME_DATA_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 1
# endif
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE
# endif
# ifndef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 700
# endif

/*
Card lib:
*/
# include "deck.h"
# include "deck_display.h"
# include "hand.h"
# include "pdisplay.h"
# include "pile_display.h"

/*
Client / Server lib:
*/
# include "sock_client_framework.h"

/*
Game related info:
*/
# include "main_statemap.h"
# include "menu_methods.h"
# include "player_action.h"
# include "game_local.h"

/*
Game manager components:
*/
# include "game_types.h"
# include "game_manager.h" 
# include "game_state_handlers.h"
# include "game_state_data.h"

/*
std::
*/
# include <sys/timerfd.h>

#endif
