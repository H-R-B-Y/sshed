
#include "hand.h"

void	_hand_remove_card_shed(struct s_hand *hand, t_card_desc card_desc)
{
	if (!hand)
		return ;
	if (hand->shed_count == 0)
		return ;
	for (unsigned int idx = 0; idx < 6; idx++)
	{
		struct s_card_plane *card_plane = hand->shed[idx];
		if (card_plane && card_is_equal(card_plane->card_desc, card_desc))
		{
			card_plane_destroy(card_plane);
			hand->shed[idx] = NULL;
			hand->shed_count--;
			if (hand->status == HAND_DISPLAY_SHED)
				hand->hand_dirty = 1;
			if (hand->shed_count == 0)
			{
				hand->card_selected[1] = -1;
				hand_update_selected(hand);
				hand->selected_card_plane = NULL;
			}
			return;
		}
	}
	return ;
}

void	hand_remove_card_shed(struct s_hand *hand, t_card_desc card_desc)
{
	_hand_remove_card_shed(hand, card_desc);
	hand_render(hand);
}
