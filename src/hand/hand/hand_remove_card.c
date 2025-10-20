
#include "hand.h"

void	_hand_remove_card(struct s_hand *hand, t_card_desc *card_desc)
{
	for (t_list *current = hand->cards; current != NULL; current = current->next)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_plane->card_desc == card_desc)
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
			hand->card_count--;
			hand->hand_dirty = 1;
			return;
		}
	}
	// Card not found in hand
	return ;
}

void	hand_remove_card(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc)
{
	(void)nc;
	_hand_remove_card(hand, card_desc);
	hand_render(nc, hand);
}

