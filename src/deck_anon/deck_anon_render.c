
#include "deck_display_anon.h"

int		deck_anon_render(
		struct notcurses *nc,
		struct s_deck_anon *deck
)
{
	if (!deck)
		return (1);
	if (!deck->is_dirty)
		return (0);
	deck_anon_clear_screen(deck);
	if (deck->hidden)
		return (deck->is_dirty = 0, 0);
	struct ncvisual_options	vopts = {
		.n = deck->plane,
		.blitter = NCBLIT_PIXEL,
		.flags = NCVISUAL_OPTION_CHILDPLANE,
		.scaling = NCSCALE_NONE,
	};
	if (!deck->visual
		&& !(deck->visual = ncvisual_blit(nc, cardbacks[0], &vopts)))
		return (1);
	if (ncplane_printf_yx(deck->plane, 0, CARD_WIDTH + 1,
		"%u", deck->card_count
		) == 0)
		return (deck_anon_clear_screen(deck), 1);
	deck->is_dirty = 0;
	return (0);
}


int		deck_anon_clear_screen(
		struct s_deck_anon *deck
)
{
	if (!deck)
		return (1);
	if (deck->visual)
	{
		ncplane_destroy(deck->visual);
		deck->visual = 0;
	}
	if (deck->plane)
		ncplane_erase(deck->plane);
	return (0);
}
