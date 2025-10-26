
# include "pile_display.h"

int	pile_top_card(struct s_pile_display *pile_display, struct s_card_desc *card)
{
	if (!pile_display || !card)
		return (1);
	if (pile_display->cards->count == 0)
		return (1);
	if (!pile_display->cards->head
		|| !pile_display->cards->head->data)
		return (1); // this is fatal if we are here we should clean exit because something is wrong
	(*card) = ((struct s_card_plane *)pile_display->cards->head->data)->card_desc;
	return (0);
}

int	pile_bottom_card(struct s_pile_display *pile_display, struct s_card_desc *card)
{
	if (!pile_display || !card)
		return (1);
	if (pile_display->cards->count == 0)
		return (1);
	if (!pile_display->cards->tail
		|| !pile_display->cards->tail->data)
		return (1);
	(*card) = ((struct s_card_plane *)pile_display->cards->tail->data)->card_desc;
	return (0);
}
