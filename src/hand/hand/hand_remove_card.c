
#include "hand.h"

void	_hand_remove_card(struct s_hand *hand, t_card_desc card_desc)
{
	int move_sel;

	if (!hand)
		return ;
	if (hand->card_count == 0)
		return ;
	else if (hand->card_count == 1)
		move_sel = -1;
	else
		move_sel = 0;
	for (t_list *current = hand->cards; current != NULL; current = current->next)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_is_equal(card_plane->card_desc, card_desc))
		{
			t_list *to_remove = current;
			if (to_remove == hand->cards)
				hand->cards = to_remove->next;
			else
			{
				t_list *prev = hand->cards;
				while (prev->next != to_remove)
					prev = prev->next;
				prev->next = to_remove->next;
			}
			card_plane_destroy(card_plane);
			return_free_list(&hand->allocator, to_remove);
			if (move_sel == -1)
				hand->card_selected[0] = -1;
			hand->card_count--;
			if (hand->card_count == 0)
			{
				hand->selected_card_plane = NULL;
				hand->card_selected[0] = -1;
			}
			hand->hand_dirty = 1;
			return;
		}
	}
	// Card not found in hand
	return ;
}

void	hand_remove_card(struct notcurses *nc, struct s_hand *hand, t_card_desc card_desc)
{
	(void)nc;
	_hand_remove_card(hand, card_desc);
	hand_render(nc, hand);
}

