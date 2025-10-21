
#include "hand.h"

void	_hand_remove_card_shed(struct s_hand *hand, t_card_desc *card_desc)
{
	int move_sel;

	if (!hand || !card_desc)
		return ;
	if (hand->shed_count == 0)
		return ;
	else if (hand->shed_count == 1)
		move_sel = -1;
	else
		move_sel = 0;
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
	if (move_sel == -1)
		hand->card_selected[1] = -1;
	return ;
}

void	hand_remove_card_shed(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc)
{
	_hand_remove_card_shed(hand, card_desc);
	hand_render(nc, hand);
}
