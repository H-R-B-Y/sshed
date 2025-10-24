

#include "pile_display.h"

int		pile_display_add_card_top(
	struct notcurses *nc,
	struct s_pile_display *pile_display,
	t_card_desc card_desc
)
{
	if (!pile_display)
		return (1);
	(void)nc;
	struct s_card_plane *card_plane = card_plane_create(card_desc);
	if (!card_plane)
		return (1);
	card_plane->is_face_down = pile_display->is_face_down;
	card_plane->orientation = (pile_display->card_orientation);
	struct s_cdll_node *new_node = cdll_init_node(card_plane);
	if (!new_node)
	{
		card_plane_destroy(card_plane);
		return (1);
	}
	cdll_push_front(pile_display->cards, new_node);
	pile_display->is_dirty = 1;
	return (0);
}

int		pile_display_add_card_bottom(
	struct notcurses *nc,
	struct s_pile_display *pile_display,
	t_card_desc card_desc
)
{
	if (!pile_display)
		return (1);
	(void)nc;
	struct s_card_plane *card_plane = card_plane_create(card_desc);
	if (!card_plane)
		return (1);
	card_plane->is_face_down = pile_display->is_face_down;
	card_plane->orientation = (pile_display->card_orientation);
	struct s_cdll_node *new_node = cdll_init_node(card_plane);
	if (!new_node)
	{
		card_plane_destroy(card_plane);
		return (1);
	}
	cdll_push_back(pile_display->cards, new_node);
	pile_display->is_dirty = 1;
	return (0);
}

int _find(struct s_card_plane *a, struct s_card_desc *b)
{
	if (card_is_equal(
		a->card_desc, *b
		))
		return (0);
	return (1);
}

int		pile_display_remove_card(
	struct notcurses *nc,
	struct s_pile_display *pile_display,
	t_card_desc card_desc
)
{
	if (!pile_display)
		return (1);
	(void)nc;
	struct s_cdll_node *node = cdll_find(pile_display->cards, &card_desc, (int (*)(void *, void *))&_find);
	if (!node)
		return (1);
	cdll_remove_node(pile_display->cards, node, (t_freefn)card_plane_destroy);
	pile_display->is_dirty = 1;
	return (0);
}
