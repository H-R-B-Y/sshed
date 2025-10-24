#include "hand.h"

void	hand_clear_screen(struct s_hand *hand)
{
	struct s_card_plane *card_plane;

	if (!hand)
		return ;
	ncplane_erase(hand->hand_plane);
	for (unsigned int idx = 0; idx < hand->shed_count; idx++)
	{
		card_plane = hand->shed[idx];
		if (card_plane && card_plane->plane)
			ncplane_erase(card_plane->plane);
	}
	for (t_list *current = hand->cards; current != NULL; current = current->next)
	{
		card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_plane->plane)
			ncplane_erase(card_plane->plane);
	}
	hand->hand_dirty = 1;
}


