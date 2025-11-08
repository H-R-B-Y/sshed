
#ifndef PLAYER_ACTION_H
# define PLAYER_ACTION_H

# include "cards.h"
# include "enum_macro.h"


# define PLAYER_ACTION_LIST(prefix, func) \
	func(prefix, NONE) \
	func(prefix, SWAP) \
	func(prefix, PLAY) \
	func(prefix, DEFAULT)

ENUM_FACTORY(PLAYER_ACTION, player_action, PLAYER_ACTION_LIST);

// Not sure where to put this, I know it shouldnt go in the main statemap header, but not sure where to put it

// This defines a move that a player or AI has chosen to take
// This will be the only way to input actions into the game loop for a turn
struct s_player_action
{
	/*
	Actions are:
		none - No action yet
		swap - Only allowed during swap phase (not implemented yet)
		play - Playing 1 to 4 cards
		default - No card played, chose to pickup the play pile
	*/
	t_player_action		action;
	/*
	Cards:
		For play action - up to 4 cards to play
		For swap action - Card should contain the card in the hand and swap card in the shed
		For default action - No cards needed
	*/
	struct s_card_desc	cards[4];
	t_u8				card_count;
	struct s_card_desc	swap_cards[4];
	t_u8				swap_card_count;
	t_u8				ready;
};

/*
Now we need to define whether these actually do the validation on
construction or whether it is on the game loop to validate the information.

I think these should not do any validation, and the validation should be done
by the game loop, these constructors just make sure that data is ordered correctly.
*/

struct s_player_action	clean_action(void);

int						action_set(
	struct s_player_action *action,
	t_player_action type
);

int						action_play_cards(
	struct s_player_action *action,
	struct s_card_desc *cards,
	t_u8 card_count
);

int						action_swap_cards(
	struct s_player_action *action, 
	struct s_card_desc *card_in_shed,
	struct s_card_desc *card_in_hand
);

int						action_default_on_cards(
	struct s_player_action *action
);

int						action_ready(
	struct s_player_action *action
);

struct s_game_manager;
struct s_game_local;
int	_handle_player_action(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
);

#endif
