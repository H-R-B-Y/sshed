
#include "card_plane.h"


int	flip_card_plane(
	struct s_card_plane *card_plane
)
{
	if (!card_plane || !card_plane->plane)
		return (1);
	card_plane->is_face_down = !card_plane->is_face_down;
	hide_card_plane(card_plane);
	show_card_plane(card_plane);
	return (0);
}

int	rotate_card_plane(
	struct s_card_plane *card_plane,
	enum e_card_orientation	new_orientation
)
{
	if (!card_plane || !card_plane->plane)
		return (1);
	if (new_orientation >= CARD_ORIENTATION_COUNT)
		return (1);
	if (card_plane->orientation == new_orientation)
		return (0); // No change needed
	card_plane->last_orientation = card_plane->orientation;
	card_plane->orientation = new_orientation;
	hide_card_plane(card_plane);
	show_card_plane(card_plane);
	return (0);
}