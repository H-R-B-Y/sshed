#include "hand.h"

void	hand_clear_screen(struct s_hand *hand)
{
	struct s_card_plane	*card_plane;
	struct s_cdll_node	*node;
	unsigned int		idx;

	if (!hand)
		return ;
	ncplane_erase(hand->hand_plane);
	for (idx = 0; idx < hand->shed_count; idx++)
	{
		card_plane = hand->shed[idx];
		if (card_plane && card_plane->plane)
			hide_card_plane(card_plane);
	}
	for ((idx = 0),(node = hand->_cards.head);
		idx < hand->_cards.count;
		(idx++), (node = node->next)
		)
	{
		card_plane = node->data;
		if (card_plane)
			hide_card_plane(card_plane);
	}
	hand->hand_dirty = 1;
}


