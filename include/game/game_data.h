
#ifndef GAME_DATA_H
# define GAME_DATA_H

# include "main_statemap.h"
# include "deck.h"
# include "deck_display.h"
# include "hand.h"
# include "pdisplay.h"
# include "pile_display.h"
# include "sock_client_framework.h"


struct s_game
{
	t_game_state	state;

	// This should be abstracted out to a sub struct
	struct s_deck	*deck;
	struct s_hand	*hand;
};

#endif
