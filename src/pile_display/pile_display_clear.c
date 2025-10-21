
#include "pile_display.h"

int pile_display_clear_screen(
	struct notcurses *nc,
	struct s_pile_display *pile_display
)
{
	if (!pile_display || !nc)
		return (1);
	ncplane_erase(pile_display->plane);
	for (struct s_cdll_node *current = pile_display->cards->head;
		current != NULL;
		current = current->next)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->data;
		if (card_plane)
			hide_card_plane(nc, pile_display->plane, card_plane);
	}
	return (0);
}

int pile_display_toggle_visibility(
	struct notcurses *nc,
	struct s_pile_display *pile_display
)
{
	if (!pile_display || !nc)
		return (1);
	pile_display_clear_screen(nc, pile_display);
	pile_display->is_visible = !pile_display->is_visible;
	pile_display->is_dirty = 1;
	pile_display_render(nc, pile_display);
	return (0);
}
