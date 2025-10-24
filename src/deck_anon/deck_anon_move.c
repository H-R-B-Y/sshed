
#include "deck_display_anon.h"

int		deck_display_anon_move(
		struct s_deck_display_anon *deck,
		unsigned int x,
		unsigned int y
)
{
	if (!deck)
		return (1);
	if (!deck->plane)
		return (1);
	ncplane_move_yx(deck->plane, y, x);
	deck->y = y;
	deck->x = x;
	return (0);
}
