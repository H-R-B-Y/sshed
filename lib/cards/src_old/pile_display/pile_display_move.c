#include "pile_display.h"

int pile_display_move(
	struct s_pile_display *pile_display,
	unsigned int new_x,
	unsigned int new_y
)
{
	if (!pile_display || !pile_display->plane)
		return (1);
	ncplane_move_yx(pile_display->plane, new_y, new_x);
	return (0);
}
