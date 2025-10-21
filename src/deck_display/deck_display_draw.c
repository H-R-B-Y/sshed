#include "deck_display.h"

t_card_desc	*deck_display_draw_top_card(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (NULL);
	deck_display->is_dirty = 1;
	return (deck_draw_card(deck_display->deck));
}


t_card_desc	*deck_display_peek_top_card(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (NULL);
	return (deck_peek_card(deck_display->deck,
		(unsigned int)(deck_display->deck->remaining - 1)));
}

int			return_card_to_deck_display(
	struct s_deck_display *deck_display,
	t_card_desc *card_desc
)
{
	if (!deck_display || !deck_display->deck || !card_desc)
		return (1);
	deck_return_card(deck_display->deck, card_desc);
	deck_display->is_dirty = 1;
	return (0);
}
