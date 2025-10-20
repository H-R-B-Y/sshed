
#ifndef GAME_H
# define GAME_H

# include <notcurses/notcurses.h>

# define FT_INCLUDE_ALL
# include "libft.h"


#include "cards.h"
#include "deck.h"
#include "hand.h"
#include "card_plane.h"
#include "pdisplay.h"

// SUBJECT TO CHANGE
int	init_client(
	struct notcurses **nc,
	struct s_deck **deck,
	struct s_hand **hand
);


#endif

