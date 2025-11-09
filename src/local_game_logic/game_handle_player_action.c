
# include "game/player_action.h"
# include "game/game_local.h"
# include "game/game_data.h"

typedef int (*t_special_action)(struct s_game_manager *manager, struct s_game_local *game, struct s_player_action *action);

int	special_action_burn_pile(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
)
{
	struct s_cdll_node	*node;
	struct s_card_plane	*plane;

	(void)manager;
	(void)action;
	while (game->pile_display->cards->count)
	{
		node = game->pile_display->cards->head;
		plane = node->data;
		pile_display_remove_card(game->pile_display, plane->card_desc);
	}
	return (0);
}

static const t_special_action	special_actions[RANK_COUNT] = {
	[RANK_ACE]				= NULL,
	[RANK_TWO]				= NULL,
	[RANK_THREE]			= NULL,
	[RANK_FOUR]				= NULL,
	[RANK_FIVE]				= NULL,
	[RANK_SIX]				= NULL,
	[RANK_SEVEN]			= NULL,
	[RANK_EIGHT]			= NULL,
	[RANK_NINE]				= NULL,
	[RANK_TEN]				= NULL,
	[RANK_JACK]				= NULL,
	[RANK_QUEEN]			= NULL,
	[RANK_KING]				= NULL,
	[RANK_JOKER_BLACK]		= special_action_burn_pile,
	[RANK_JOKER_RED]		= special_action_burn_pile
};

static int	_validate_player_turn(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
)
{
	struct s_pdisplay	*p;

	(void)manager; // this would later be used to check if the game rules have been set
	// what happens if validation fails
	// Like how do we actually cope with that
	if (game->play_state == PLAY_STATE_SWAP_PHASE
		&& !(action->action == PLAYER_ACTION_DEFAULT
		|| action->action == PLAYER_ACTION_SWAP))
		return (1);
	else if (game->play_state == PLAY_STATE_PLAY_PHASE
		&& !(action->action == PLAYER_ACTION_DEFAULT
		|| action->action == PLAYER_ACTION_PLAY))
		return (1);
	else if (!(game->play_state == PLAY_STATE_PLAY_PHASE
		|| game->play_state == PLAY_STATE_SWAP_PHASE))
		return (1); // cannot do anything in these phases
	if ((action->action == PLAYER_ACTION_PLAY
		&& !(action->card_count > 0 || action->card_count < 5))
		|| (action->action == PLAYER_ACTION_SWAP && action->card_count != 2))
		return (1);
	if (game->whos_turn == 0)
	{
		// Just going to validate the first card until i figure out how to do
		// multiple card plays
		// TODO: validate all cards
		if (!hand_contains_card(game->hand, action->cards[0]))
			return (1);
		if (game->hand->card_count > 0 && hand_card_in_shed(game->hand, action->cards[0]))
			return (1);
		else if (game->hand->card_count == 0 && hand_card_in_hand(game->hand, action->cards[0]))
			return (1); // This shouldnt need to be validated but whatever
	}
	else
	{
		p = game->pdisplay[game->whos_turn - 1];
		if (!pdisplay_has_card(p, action->cards[0]))
			return (1);
		if (p->card_count > 0 && pdisplay_card_is_shed(p, action->cards[0]))
			return (1);
		else if (p->card_count == 0 && pdisplay_card_is_hand(p, action->cards[0]))
			return (1);
	}
	return (0);
}

// static int	_lower_check(
// 	struct s_game_manager *manager,
// 	struct s_card_desc pile_card,
// 	struct s_card_desc hand_card
// )
// {
// 	// defunct
// 	(void)manager;
// 	(void)pile_card;
// 	(void)hand_card;
// 	return (0);
// }

static int	_next_card_stacks(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
)
{
	t_card_desc	*what_is_on_top;

		(void)manager;
	if (game->pile_display->cards->count)
		what_is_on_top = &((struct s_card_plane *)game->pile_display->cards->head->data)->card_desc;
	else
		what_is_on_top = NULL;
	if (what_is_on_top == NULL)
		return (1); // can always play to an empty pile
	if (action->cards[0].rank == RANK_ACE)
		return (1); // can always play ace they are always lower and higher
	else if (action->cards[0].rank == RANK_EIGHT)
		return (1); // eight is invisible
	else if (action->cards[0].rank == RANK_SEVEN)
		return (1); // eight is invisible
	else if (action->cards[0].rank == RANK_JOKER_BLACK
		|| action->cards[0].rank == RANK_JOKER_RED)
		return (1); // always play a joker
	else if (what_is_on_top->rank == RANK_SEVEN)
	{
		if (action->cards[0].rank <= RANK_SEVEN)
			return (1); // Seven must be lower
		else
			return (0);
	}
	else if (what_is_on_top->rank == RANK_EIGHT)
	{
		if (game->pile_display->cards->count == 1)
			return (action->cards[0].rank >= what_is_on_top->rank);

		size_t				idx;
		struct s_card_plane	*plane;
		struct s_cdll_node	*node;

		idx = 1;
		node = game->pile_display->cards->head->next;
		plane = node->data;
		while (idx < game->pile_display->cards->count
			&& plane->card_desc.rank == RANK_EIGHT)
		{
			node = node->next;
			idx++;
			plane = node->data;
		}
		if (plane->card_desc.rank == RANK_EIGHT)
			return (action->cards[0].rank >= what_is_on_top->rank);
		if (plane->card_desc.rank == RANK_SEVEN
			&& action->cards[0].rank <= RANK_SEVEN)
			return (1);
		else if (plane->card_desc.rank <= action->cards[0].rank)
			return (1);
	}
	else if (action->cards[0].rank == what_is_on_top->rank)
		return (1);
	else if (action->cards[0].rank >= what_is_on_top->rank)
		return (1);
	return (0);
}

/*
Validation:
 - It is the players turn (this doesn't matter)
 - It is the correct game state for the action
 - The card is owned by the player
 - The card comes from the right list (not shed when hand has cards)

Then:
 - Is the card higher than the card it was played on (or special)
 - If the card is special, perform the special action
 - If the card is not special and is not higher or equal
	- Add the pile to the players hand

All of these actions need to be added to the game log,
we need to figure out the best way to structure the game log.

I want it to be easy to create logs, but each state of the log struct
has a string value that can be shown to the user, and is verbose enough
for anyone to know the entire state of play from looking at the log.

*/

static int remove_action_from_hand(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action,
	int is_pdisplay,
	void *hand
)
{
	size_t	i;
	struct s_pdisplay	*pdisplay;
	struct s_hand		*hand_struct;

	(void)manager;
	(void)game;
	i = 0;
	while (i < action->card_count)
	{
		if (is_pdisplay)
		{
			pdisplay = (struct s_pdisplay *)hand;
			if (pdisplay->card_count)
				pdisplay_remove_card(pdisplay, action->cards[i]);
			else
				pdisplay_remove_card_shed(pdisplay, action->cards[i]);
		}
		else
		{
			hand_struct = (struct s_hand *)hand;
			if (hand_struct->card_count)
				hand_remove_card(hand_struct, action->cards[i]);
			else
				hand_remove_card_shed(hand_struct, action->cards[i]);
		}
		i++;
	}
	return (0);
}

static int move_action_to_pile(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
)
{
	size_t	i;

	(void)manager;
	i = 0;
	while (i < action->card_count)
	{
		pile_display_add_card_top(game->pile_display, action->cards[i]);
		i++;
	}
	return (0);
}

static int populate_hand(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
)
{
	struct s_pdisplay	*pdisplay;

	(void)manager;
	(void)action;
	if (game->deck_display->deck->remaining == 0)
		return (0);
	if (game->whos_turn > 0)
		pdisplay = game->pdisplay[game->whos_turn - 1];
	while (game->deck_display->deck->remaining
		&& ((game->whos_turn == 0 && game->hand->card_count < 5)
		|| (game->whos_turn > 0 && pdisplay->card_count < 5)))
	{
		struct s_card_desc	card;
		if (deck_display_draw_top_card(game->deck_display, &card))
			break ;
		if (game->whos_turn == 0)
			hand_add_card(game->hand, card);
		else
			pdisplay_add_card(pdisplay, card);
	}
	return (0);
}


int _handle_player_player_action(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
)
{
	struct s_pdisplay	*pdisplay;


	if (_validate_player_turn(manager, game, action))
		return (((*action) = clean_action()), 0);
	if (game->whos_turn > 0)
		pdisplay = game->pdisplay[game->whos_turn - 1];
	if (_next_card_stacks(manager, game, action))
	{
		remove_action_from_hand(manager, game, action,
			(game->whos_turn > 0), (game->whos_turn > 0) ? (void *)pdisplay : (void *)game->hand);
		move_action_to_pile(manager, game, action);
		if (special_actions[action->cards[0].rank])
			special_actions[action->cards[0].rank](manager, game, action);
		populate_hand(manager, game, action);
	}
	else
	{
		remove_action_from_hand(manager, game, action,
			(game->whos_turn > 0), (game->whos_turn > 0) ? (void *)pdisplay : (void *)game->hand);
		move_action_to_pile(manager, game, action);
		if (game->whos_turn == 0)
		{
			hand_show_hand(game->hand);
			pile_display_return_to_hand(game->pile_display, false, game->hand);
		}
		else
		{
			pdisplay_show_hand(pdisplay);
			pile_display_return_to_hand(game->pile_display, true, pdisplay);
		}
	}
	return (0);
}


int	_handle_player_action(
	struct s_game_manager *manager,
	struct s_game_local *game,
	struct s_player_action *action
)
{
	if (!manager || !game || !action)
		return (1);
	switch (action->action)
	{
		case PLAYER_ACTION_PLAY:
			return (_handle_player_player_action(manager, game, action));
		case PLAYER_ACTION_SWAP:
			return (MANAGER_RET_ERR("Swap action no longer supported through action interface"));
		case PLAYER_ACTION_DEFAULT:
			if (game->whos_turn == 0)
				pile_display_return_to_hand(game->pile_display, false, game->hand);
			else
				pile_display_return_to_hand(game->pile_display, true, game->pdisplay[game->whos_turn - 1]);
			return (0);
		case PLAYER_ACTION_NONE:
			return (0);
		default:
			break ;
	}
	return (0);
}

