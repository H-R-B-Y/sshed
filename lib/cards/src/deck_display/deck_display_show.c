
#include "deck_display.h"

int			deck_display_hide(
	struct s_deck_display *deck_display
)
{
	if (!deck_display)
		return (1);
	deck_display_clear_screen(deck_display);
	deck_display->hidden = 1;
	deck_display->is_dirty = 1;
	return (0);
}

int			deck_display_show(
	struct s_deck_display *deck_display
)
{
	if (!deck_display)
		return (1);
	deck_display->hidden = 0;
	deck_display->is_dirty = 1;
	return (0);
}

int			deck_display_toggle_visibility(
	struct s_deck_display *deck_display
)
{
	if (!deck_display)
		return (1);
	if (!deck_display->hidden)
		return (deck_display_hide(deck_display));
	else
		return (deck_display_show(deck_display));
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
	ncplane_move_yx(deck_display->plane, (int)new_y, (int)new_x);
	deck_display->is_dirty = 1;
	return (0);
}
