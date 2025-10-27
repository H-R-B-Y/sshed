
#include "pdisplay.h"

int pdisplay_show_hand(struct s_pdisplay *pdisplay)
{
	if (!pdisplay)
		return (1);
	if (pdisplay->status != PDISPLAY_HAND)
	{
		pdisplay->status = PDISPLAY_HAND;
		for (unsigned int idx = 0; idx < 6; idx++)
		{
			struct s_card_plane *card_plane = pdisplay->shed[idx];
			if (card_plane)
				hide_card_plane(card_plane);
		}
		pdisplay->pdisplay_dirty = 1;
		return (0);
	}
	return (1);
}

int pdisplay_show_shed(struct s_pdisplay *pdisplay)
{
	if (!pdisplay)
		return (1);
	if (pdisplay->status != PDISPLAY_SHED)
	{
		pdisplay->status = PDISPLAY_SHED;
		for (t_list *current = pdisplay->cards; current != NULL; current = current->next)
		{
			struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
			hide_card_plane(card_plane);
		}
		pdisplay->pdisplay_dirty = 1;
		return (0);
	}
	return (1);
}

int pdisplay_toggle_display(struct s_pdisplay *pdisplay)
{
	if (!pdisplay)
		return (1);
	pdisplay_clear_screen(pdisplay);
	if (pdisplay->status == PDISPLAY_HAND)
		pdisplay_show_shed(pdisplay);
	else
		pdisplay_show_hand(pdisplay);
	pdisplay_render(pdisplay);
	return (0);
}

