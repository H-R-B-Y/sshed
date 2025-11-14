
# include "hand.h"

static int	card_compare(struct s_card_plane *a, struct s_card_plane *b)
{
	return (b->card_desc.rank - a->card_desc.rank);
}

int	hand_sort_cards(struct s_hand *hand)
{
	if (!hand)
		return (1);
	t_list *head = ft_lstsort(hand->cards, (void *)card_compare);
	if (head)
		hand->cards = head;
	hand->hand_dirty = 1;
	hand_update_selected(hand);
	return (0);
}
