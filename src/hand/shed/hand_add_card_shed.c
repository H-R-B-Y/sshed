
#include "hand.h"


int		_hand_add_card_to_shed(struct s_hand *hand, t_card_desc *card_desc)
{
	struct s_card_plane	*card_plane;

	if (!hand || !card_desc)
		return (1);
	if (hand->shed_count >= 6)
		return (1); // Shed is full
	card_plane = card_plane_create(hand, card_desc);
	if (!card_plane)
		return (1);
	card_plane->is_face_down = (hand->shed_count < 3) ? 1 : 0;
	hand->shed[hand->shed_count] = card_plane;
	hand->shed_count++;
	if (hand->status == HAND_DISPLAY_SHED)
		hand->hand_dirty = 1;
	return (0);
}

int		hand_add_card_to_shed(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc)
{
	int	ret;
	hand_clear_screen(nc, hand);
	ret = _hand_add_card_to_shed(hand, card_desc);
	hand_render(nc, hand);
	return (ret);
}
