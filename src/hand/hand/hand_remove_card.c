
#include "hand.h"

int		_unlink_node(t_cdll *list, struct s_cdll_node *node)
{
	if (list->count == 0)
		return (0);
	if (list->count == 1)
	{
		node->next = 0;
		node->prev = 0;
		list->head = 0;
		list->tail = 0;
	}
	else
	{
		if (node == list->head)
			list->head = node->next;
		if (node == list->tail)
			list->tail = node->prev;
		node->next->prev = node->prev;
		node->prev->next = node->next;
		node->next = 0;
		node->prev = 0;
	}
	list->count--;
	return (0);
}

void	_hand_remove_card(struct s_hand *hand, t_card_desc card_desc)
{
	unsigned int		idx;
	struct s_cdll_node	*node;
	struct s_card_plane	*card_plane;

	if (!hand)
		return ;
	if (hand->card_count == 0)
		return ;
	idx = 0;
	node = hand->_cards.head;
	while (idx < hand->_cards.count)
	{
		card_plane = node->data;
		if (card_plane && card_is_equal(card_plane->card_desc, card_desc))
		{
			_unlink_node(&hand->_cards, node);
			card_plane_destroy(card_plane);
			node->data = 0;
			free(node);
			hand->card_count--;
			hand->hand_dirty = 1;
			return ;
		}
		idx++;
		node = node->next;
	}
	return ;
}


void	hand_remove_card(struct s_hand *hand, t_card_desc card_desc)
{
	_hand_remove_card(hand, card_desc);
	hand_render(hand);
}

