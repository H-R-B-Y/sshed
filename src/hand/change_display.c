
#include "hand.h"

void	hand_show_hand(struct notcurses *nc, struct s_hand *hand)
{
	if (!hand || !nc)
		return ;
	if (hand->status != HAND_DISPLAY_HAND)
	{
		hand->status = HAND_DISPLAY_HAND;
		for (unsigned int idx = 0; idx < hand->shed_count; idx++)
		{
			struct s_card_plane *card_plane = hand->shed[idx];
			hide_card_plane(nc, hand->hand_plane, card_plane);
		}
		hand->hand_dirty = 1;
	}
}

void	hand_show_shed(struct notcurses *nc, struct s_hand *hand)
{
	if (!hand || !nc)
		return ;
	if (hand->status != HAND_DISPLAY_SHED)
	{
		hand->status = HAND_DISPLAY_SHED;
		for (t_list *current = hand->cards; current != NULL; current = current->next)
		{
			struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
			hide_card_plane(nc, hand->hand_plane, card_plane);
		}
		hand->hand_dirty = 1;
	}
}

void	hand_toggle_display(struct notcurses *nc, struct s_hand *hand)
{
	if (!hand || !nc)
		return ;
	hand_clear_screen(nc, hand);
	if (hand->status == HAND_DISPLAY_HAND)
		hand_show_shed(nc, hand);
	else
		hand_show_hand(nc, hand);
	hand_render(nc, hand);
}
