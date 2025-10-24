
#include "hand.h"

int	_hand_add_card(struct s_hand *hand, t_card_desc card_desc)
{
	struct s_card_plane	*card_plane;
	t_list				*card_list_node;
	int					move_sel;

	if (!hand)
		return (1);
	if (hand->card_count == 0)
		move_sel = 1;
	else
		move_sel = 0;
	card_plane = card_plane_create(card_desc);
	if (!card_plane)
		return (1);
	card_list_node = checkout_free_list(&hand->allocator, sizeof(t_list));
	if (!card_list_node)
	{
		card_plane_destroy(card_plane);
		return (1);
	}
	card_list_node->content = card_plane;
	card_list_node->next = NULL;
	ft_lstadd_back(&hand->cards, card_list_node);
	hand->card_count++;
	if (move_sel)
		hand->card_selected[0] = 0;
	hand->hand_dirty = 1;
	return (0);
}

int	hand_add_card(struct notcurses *nc, struct s_hand *hand, t_card_desc card_desc)
{
	int	ret;
	(void)nc;
	ret = _hand_add_card(hand, card_desc);
	hand_render(nc, hand);
	return (ret);
}

