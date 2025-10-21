
#include "deck_display.h"

int			deck_display_hide(
	struct notcurses *nc,
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !nc)
		return (1);
	ncplane_erase(deck_display->plane);
	deck_display->is_visible = 0;
	deck_display->is_dirty = 1;
	return (0);
}

int			deck_display_show(
	struct notcurses *nc,
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !nc)
		return (1);
	struct ncvisual_options vopts = {
		.n = deck_display->plane,
		.x = 1,
		.y = 1,
		.blitter = NCBLIT_PIXEL,
		.scaling = NCSCALE_SCALE,
	};
	ncvisual_blit(nc, cardbacks[0], &vopts);
	deck_display->is_visible = 1;
	deck_display->is_dirty = 1;
	return (0);
}

int			deck_display_toggle_visibility(
	struct notcurses *nc,
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !nc)
		return (1);
	if (deck_display->is_visible)
		return (deck_display_hide(nc, deck_display));
	else
		return (deck_display_show(nc, deck_display));
}

int		deck_display_move(
	struct s_deck_display *deck_display,
	unsigned int new_x,
	unsigned int new_y
)
{
	if (!deck_display)
		return (1);
	deck_display->x = new_x;
	deck_display->y = new_y;
	ncplane_yx(deck_display->plane, (int)new_x, (int)new_y);
	deck_display->is_dirty = 1;
	return (0);
}