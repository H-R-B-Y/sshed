
#include "hand.h"

void	hand_show_hand(struct s_hand *hand)
{
	if (!hand)
		return ;
	if (hand->status != HAND_DISPLAY_HAND)
	{
		hand->status = HAND_DISPLAY_HAND;
		for (unsigned int idx = 0; idx < 6; idx++)
		{
			struct s_card_plane *card_plane = hand->shed[idx];
			if (card_plane)
				hide_card_plane(card_plane);
		}
		hand->hand_dirty = 1;
	}
}

void	hand_show_shed(struct s_hand *hand)
{
	if (!hand)
		return ;
	if (hand->status != HAND_DISPLAY_SHED)
	{
		hand->status = HAND_DISPLAY_SHED;
		for (t_list *current = hand->cards; current != NULL; current = current->next)
		{
			struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
			hide_card_plane(card_plane);
		}
		hand->hand_dirty = 1;
	}
}

void	hand_toggle_display(struct s_hand *hand)
{
	if (!hand)
		return ;
	hand_clear_screen(hand);
	if (hand->status == HAND_DISPLAY_HAND)
		hand_show_shed(hand);
	else
		hand_show_hand(hand);
	hand_render(hand);
}
