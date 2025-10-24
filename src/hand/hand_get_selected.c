
#include "hand.h"

struct s_card_plane	*_hand_get_selected_card_plane(struct s_hand *hand)
{
	if (!hand)
		return (NULL);
	if (hand->status == HAND_DISPLAY_HAND)
	{
		if (hand->card_selected[0] < 0 || hand->card_selected[0] >= (int)hand->card_count)
			return (NULL);
		t_list *current = hand->cards;
		int idx = 0;
		while (current)
		{
			if (idx == hand->card_selected[0])
				return ((struct s_card_plane *)current->content);
			current = current->next;
			idx++;
		}
	}
	else if (hand->status == HAND_DISPLAY_SHED)
	{
		if (hand->card_selected[1] < 0 || hand->card_selected[1] >= (int)3)
			return (NULL);
		if (hand->shed[hand->card_selected[1] + 3])
			return (hand->shed[hand->card_selected[1] + 3]);
		else 
			return (hand->shed[hand->card_selected[1]]);
	}
	return (NULL);
}


int	hand_select_next_card(struct s_hand *hand)
{
	if (!hand)
		return (1);
	if (hand->status == HAND_DISPLAY_HAND)
	{
		if (hand->card_count == 0)
			return (hand->card_selected[0] = -1, 0);
		hand->card_selected[0] = (hand->card_selected[0] + 1) % hand->card_count;
		hand->selected_card_plane = _hand_get_selected_card_plane(hand);
	}
	else if (hand->status == HAND_DISPLAY_SHED)
	{
		if (hand->shed_count == 0)
			return (hand->card_selected[1] = -1, 0);
		hand->card_selected[1] = (hand->card_selected[1] + 1) % (6 / 2);
		while (hand->shed[hand->card_selected[1]] == NULL)
			hand->card_selected[1] = (hand->card_selected[1] + 1) % (6 / 2);
		hand->selected_card_plane = _hand_get_selected_card_plane(hand);
	}
	hand->hand_dirty = 1;
	return (0);
}

int	hand_select_prev_card(struct s_hand *hand)
{
	if (!hand)
		return (1);
	if (hand->status == HAND_DISPLAY_HAND)
	{
		if (hand->card_count == 0)
			return (hand->card_selected[0] = -1, 0);
		hand->card_selected[0]--;
		if (hand->card_selected[0] < 0)
			hand->card_selected[0] = hand->card_count - 1;
		hand->selected_card_plane = _hand_get_selected_card_plane(hand);
	}
	else if (hand->status == HAND_DISPLAY_SHED)
	{
		if (hand->shed_count == 0)
			return (hand->card_selected[1] = -1, 0);
		hand->card_selected[1]--;
		if (hand->card_selected[1] < 0)
			hand->card_selected[1] = (6 / 2) - 1;
		while (hand->shed[hand->card_selected[1]] == NULL)
		{
			hand->card_selected[1]--;
			if (hand->card_selected[1] < 0)
				hand->card_selected[1] = (6 / 2) - 1;
		}
		hand->selected_card_plane = _hand_get_selected_card_plane(hand);
	}
	hand->hand_dirty = 1;
	return (1);
}

int	hand_update_selected(struct s_hand *hand)
{
	if (!hand)
		return (1);
	if (hand->status == HAND_DISPLAY_HAND)
	{
		if (hand->card_count == 0)
			return (hand->card_selected[1] = -1, 0);
		while (hand->card_selected[0] < 0 || hand->card_selected[0] >= (int)hand->card_count)
			hand->card_selected[0]--;
		return (0);
	}
	else if (hand->status == HAND_DISPLAY_SHED)
	{
		if (hand->shed_count == 0)
			return (hand->card_selected[1] = -1, 0);
		while (hand->shed[hand->card_selected[1]] == NULL)
		{
			hand->card_selected[1]--;
			if (hand->card_selected[1] < 0)
				hand->card_selected[1] = (6 / 2) - 1;
		}
		return (0);
	}
	return (1);
}

int	hand_pop_selected_card(struct s_hand *hand, struct s_card_desc *popped_card)
{
	struct s_card_plane *selected_card_plane = NULL;
	struct s_card_desc card_desc;
	if (!hand)
		return (1);
	selected_card_plane =  _hand_get_selected_card_plane(hand);
	if (!selected_card_plane)
		return (1);
	card_desc = selected_card_plane->card_desc;
	if (hand->status == HAND_DISPLAY_HAND)
		_hand_remove_card(hand, card_desc);
	else if (hand->status == HAND_DISPLAY_SHED)
		_hand_remove_card_shed(hand, card_desc);
	hand_update_selected(hand);
	(*popped_card) = card_desc;
	hand->selected_card_plane = NULL;
	return (0);
}
