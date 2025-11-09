
#include "pile_display.h"

int pile_display_clear_screen(
	struct s_pile_display *pile_display
)
{
	size_t				idx;
	struct s_cdll_node	*current;
	struct s_card_plane *card_plane;

	if (!pile_display)
		return (1);
	ncplane_erase(pile_display->plane);
	current = pile_display->cards->head;
	idx = 0;
	while (idx < pile_display->cards->count)
	{
		card_plane = (struct s_card_plane *)current->data;
		if (card_plane)
			hide_card_plane(card_plane);
		current = current->next;
		idx++;
	}
	return (0);
}

int pile_display_hide(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return (1);
	pile_display_clear_screen(pile_display);
	pile_display->is_visible = 0;
	pile_display->is_dirty = 1;
	return (0);
}

int pile_display_show(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return (1);
	pile_display_clear_screen(pile_display);
	pile_display->is_visible = 1;
	pile_display->is_dirty = 1;
	pile_display_render(pile_display);
	return (0);
}

int pile_display_toggle_visibility(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return (1);
	pile_display_clear_screen(pile_display);
	pile_display->is_visible = !pile_display->is_visible;
	pile_display->is_dirty = 1;
	pile_display_render(pile_display);
	return (0);
}
