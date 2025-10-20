
#include "hand.h"

void	_hand_remove_card_shed(struct s_hand *hand, t_card_desc *card_desc)
{
	for (unsigned int idx = 0; idx < 6; idx++)
	{
		struct s_card_plane *card_plane = hand->shed[idx];
		if (card_plane && card_plane->card_desc == card_desc)
		{
			card_plane_destroy(card_plane);
			hand->shed[idx] = NULL;
			hand->shed_count--;
			if (hand->status == HAND_DISPLAY_HAND)
				hand->hand_dirty = 1;
			return;
		}
	}
	return ;
}

void	hand_remove_card_shed(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc)
{
	_hand_remove_card_shed(hand, card_desc);
	hand_render(nc, hand);
}
