
#include "deck_display.h"

/*
Later we want to give the effect of a stack of cards,
but for now lets just render a single card back.
*/
int deck_display_render(
	struct s_deck_display *deck_display
)
{
	struct notcurses	*nc;
	unsigned int		remaining_cards;

	if (!deck_display)
		return (1);
	if (!deck_display->is_dirty)
		return (0);
	if (deck_display->hidden)
		return (0);
	deck_display_clear_screen(deck_display);
	remaining_cards = deck_display_remaining_cards(deck_display);
	nc = ncplane_notcurses(deck_display->plane);
	struct ncvisual_options vopts = {
		.n = deck_display->plane,
		.x = 0,
		.y = 0,
		.blitter = NCBLIT_PIXEL,
		.scaling = NCSCALE_NONE,
		.flags = NCVISUAL_OPTION_CHILDPLANE,
	};
	if (remaining_cards
		&& !(deck_display->visual = ncvisual_blit(nc, cardbacks[0], &vopts)))
		return (1);
	if (remaining_cards
		&& ncplane_printf_yx(deck_display->plane, 0, CARD_WIDTH, 
		"%u", deck_display_remaining_cards(deck_display)) == 0)
		return (1);
	deck_display->is_dirty = 0;
	return (0);
}

int			deck_display_clear_screen(
	struct s_deck_display *deck_display
)
{
	if (!deck_display)
		return (1);
	if (deck_display->visual)
	{
		ncplane_destroy(deck_display->visual);
		deck_display->visual = 0;
	}
	if (deck_display->plane)
		ncplane_erase(deck_display->plane);
	deck_display->is_dirty = 1;
	return (0);
}
