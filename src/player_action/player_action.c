
# include "game/player_action.h"


struct s_player_action	clean_action(void)
{
	return (
		(struct s_player_action){
			0
		}
	);
}

int						action_set(
	struct s_player_action *action,
	t_player_action type
)
{
	if (!action)
		return (1);
	action->action = type;
	return (0);
}

int						action_play_cards(
	struct s_player_action *action,
	struct s_card_desc *cards,
	t_u8 card_count
)
{
	t_u8	idx;

	if (!action)
		return (1);
	if (!cards)
	{
		action->card_count = 0;
		return (1);
	}
	if (card_count > 4)
	{
		action->card_count = 0;
		return (1);
	}
	action->action = PLAYER_ACTION_PLAY;
	idx = 0;
	while (idx < card_count)
	{
		action->cards[idx] = cards[idx];
		idx++;
	}
	action->card_count = card_count;
	return (0);
}

int						action_swap_cards(
	struct s_player_action *action, 
	struct s_card_desc *card_in_shed,
	struct s_card_desc *card_in_hand
)
{
	if (!action)
		return (1);
	if (!card_in_hand || !card_in_shed)
		return (1);
	action->action = PLAYER_ACTION_SWAP;
	action->cards[0] = (*card_in_shed);
	action->cards[1] = (*card_in_hand);
	action->card_count = 2;
	return (0);
}

int						action_default_on_cards(
	struct s_player_action *action
)
{
	if (!action)
		return (1);
	action->action = PLAYER_ACTION_DEFAULT;
	action->card_count = 0;
	return (0);
}

int						action_ready(
	struct s_player_action *action
)
{
	if (!action)
		return (1);
	if (action->action == PLAYER_ACTION_NONE)
		return (0);
	action->ready = 1;
	return (0);
}