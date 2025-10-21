
#include "deck_display.h"

/*
Later we want to give the effect of a stack of cards,
but for now lets just render a single card back.
*/
int deck_display_render(
	struct notcurses *nc,
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !nc)
		return (1);
	if (!deck_display->is_visible)
		return (0);
	if (!deck_display->is_dirty)
		return (0);
	struct ncvisual_options vopts = {
		.n = deck_display->plane,
		.x = 1,
		.y = 1,
		.blitter = NCBLIT_PIXEL,
		.scaling = NCSCALE_SCALE,
	};
	ncvisual_blit(nc, cardbacks[0], &vopts);
	deck_display->is_dirty = 0;
	return (0);
}
