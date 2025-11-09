
#include "hand.h"

void	_hand_remove_card(struct s_hand *hand, t_card_desc card_desc)
{
	if (!hand)
		return ;
	if (hand->card_count == 0)
		return ;
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
			// return_free_list(&hand->allocator, to_remove);
			free(to_remove);
			hand->card_count--;
			if (hand->card_selected[0] >= (int)hand->card_count)
				hand_select_prev_card(hand);
			else
				hand_update_selected(hand);
			hand->hand_dirty = 1;
			return;
		}
	}
	// Card not found in hand
	return ;
}

void	hand_remove_card(struct s_hand *hand, t_card_desc card_desc)
{
	_hand_remove_card(hand, card_desc);
	hand_render(hand);
}

