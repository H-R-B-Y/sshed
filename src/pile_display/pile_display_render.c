

#include "pile_display.h"

# define min(a,b) (((a)<(b))?(a):(b))

int		pile_display_render(
	struct notcurses *nc,
	struct s_pile_display *pile_display
)
{
	if (!pile_display || !nc)
		return (1);
	if (!pile_display->is_visible)
		return (0);
	if (!pile_display->is_dirty)
		return (0);
	/*
	So we want to render the cards up to max_stack or card count whichever is smaller.
	*/
	notcurses_refresh(nc, NULL, NULL);
	unsigned int idx = 0;
	struct s_card_plane *last = NULL;
	for (struct s_cdll_node *current = pile_display->cards->head;
		current != NULL && idx < min(pile_display->max_stack, pile_display->cards->count);
		last = current->data, current = current->next, idx++)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->data;
		if (card_plane && card_plane->card_desc)
		{
			if (!card_plane->plane_shown)
				redisplay_card(nc, pile_display->plane, card_plane);
			if (last)
				ncplane_move_below(card_plane->plane, last->plane);
			ncplane_move_yx(card_plane->plane,
				(pile_display->orientation == PILE_DISPLAY_VERTICAL)
				? idx * pile_display->padding
				: 0,
				(pile_display->orientation == PILE_DISPLAY_HORIZONTAL)
				? idx * pile_display->padding
				: 0
			);
		}
	}
	if (pile_display->cards->count > pile_display->max_stack)
	{
		ncplane_printf_yx(
			pile_display->plane,
			0,
			(pile_display->orientation == PILE_DISPLAY_HORIZONTAL)
			? (pile_display->max_stack - 1) * pile_display->padding
			+ ((pile_display->card_orientation == CARD_ORIENTATION_HORIZONTAL) ? CARD_H_WIDTH : CARD_WIDTH)
			+ 1
			: ((pile_display->card_orientation == CARD_ORIENTATION_HORIZONTAL) ? CARD_H_WIDTH : CARD_WIDTH)
			+ 1,
			"+%zu",
			pile_display->cards->count - pile_display->max_stack
		);
	}
	pile_display->is_dirty = 0;
	return (0);
}
