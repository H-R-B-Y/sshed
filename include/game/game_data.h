/**
 * @file game_data.h
 * @brief Main include aggregator for game system headers
 * 
 * This is a convenience header that includes all game-related headers
 * in the correct order to avoid dependency issues. It serves as a
 * "one-stop-shop" for source files that need access to the game system.
 * 
 * Include order is important to resolve dependencies:
 * 1. System headers and feature test macros
 * 2. External library headers (card lib, networking, notcurses)
 * 3. Core game type definitions
 * 4. Game state enumerations
 * 5. Data structures for specific states
 * 6. State handler declarations
 * 7. Specialized subsystems (AI, player actions)
 */

#ifndef GAME_DATA_H
# define GAME_DATA_H

/* ========================================================================== */
/*                     POSIX/XOPEN FEATURE TEST MACROS                        */
/* ========================================================================== */

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 1
# endif
# ifndef _POSIX_SOURCE
#  define _POSIX_SOURCE
# endif
# ifndef _XOPEN_SOURCE
#  define _XOPEN_SOURCE 700
# endif

/* ========================================================================== */
/*                         EXTERNAL LIBRARY HEADERS                           */
/* ========================================================================== */

/* Card game library */
# include "deck.h"
# include "deck_display.h"
# include "hand.h"
# include "pdisplay.h"
# include "pile_display.h"

/* Client/Server framework (for future online multiplayer) */
# include "sock_client_framework.h"

/* System headers */
# include <sys/timerfd.h>

/* ========================================================================== */
/*                         CORE GAME SYSTEM HEADERS                           */
/* ========================================================================== */

/* Core types and function pointers */
# include "game_types.h"

/* Game state enumeration */
# include "main_statemap.h"

/* Game manager (main control structure) */
# include "game_manager.h"

/* State handler function declarations */
# include "game_state_handlers.h"

/* ========================================================================== */
/*                      STATE DATA STRUCTURE HEADERS                          */
/* ========================================================================== */

/* General state data (setup, settings) */
# include "game_state_data.h"

/* Menu system */
# include "menu_methods.h"
# include "menu.h"

/* Local game states (play, pause, end) */
# include "game_local.h"

/* ========================================================================== */
/*                      GAMEPLAY SUBSYSTEM HEADERS                            */
/* ========================================================================== */

/* Player action system */
# include "player_action.h"

/* AI opponent system */
# include "ai_state_machine.h"

#endif
