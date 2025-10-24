
#ifndef GAME_H
# define GAME_H

# include <notcurses/notcurses.h>

# define FT_INCLUDE_ALL
# include "libft.h"

// Status: ongoing development
#include "cards.h"

// Status: working
#include "deck.h"

// Status: working (testing for edge cases needed)
#include "hand.h"

// Status: working (api needs expanding)
#include "card_plane.h"

// Status: ongoing development
#include "pdisplay.h"

// Status: ongoing development
#include "pile_display.h"

#include "deck_display_anon.h"


// SUBJECT TO CHANGE
int	init_client(
	struct notcurses **nc,
	struct s_deck **deck,
	struct s_hand **hand
);


#endif

