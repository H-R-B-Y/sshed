
#include "hand.h"


int		_hand_add_card_to_shed(struct s_hand *hand, t_card_desc card_desc)
{
	struct s_card_plane	*card_plane;
	int					move_sel;

	if (!hand)
		return (1);
	if (hand->shed_count >= 6)
		return (1); // Shed is full
	else if (hand->shed_count == 0)
		move_sel = 1;
	else
		move_sel = 0;
	card_plane = card_plane_create(hand->hand_plane, card_desc);
	if (!card_plane)
		return (1);
	card_plane->is_face_down = (hand->shed_count < 3) ? 1 : 0;
	hand->shed[hand->shed_count] = card_plane;
	hand->shed_count++;
	if (move_sel)
		hand->card_selected[1] = 0;
	hand_update_selected(hand);
	if (hand->status == HAND_DISPLAY_SHED)
		hand->hand_dirty = 1;
	return (0);
}

int		hand_add_card_to_shed(struct s_hand *hand, t_card_desc card_desc)
{
	int	ret;
	ret = _hand_add_card_to_shed(hand, card_desc);
	hand_render(hand);
	return (ret);
}
