
# include "pile_display.h"
# include "hand.h"
# include "pdisplay.h"

// TODO: maybe return a negative on error? OMG, negative count, so we know how many were pushed but also know it was an error!!!!!

int	_pile_display_return_to_hand(
	struct s_pile_display *pile_display,
	struct s_hand *hand
)
{
	unsigned int	count = 0;

	while (pile_display->cards->count)
	{
		struct s_cdll_node *node = NULL;
		struct s_card_plane *plane = NULL;
		struct s_card_desc card;
		node = cdll_pop_front(pile_display->cards);
		if (!node)
			return (0); // TODO: what do we do here 
		plane = node->data;
		if (!plane)
			return (0); // TODO: what do we do here
		card = plane->card_desc;
		card_plane_destroy(plane);
		free(node);
		if (hand_add_card(hand, card))
			return (dprintf(STDERR_FILENO, "Failed to add card to hand\n"), 0);
		count++;
	}
	hand->hand_dirty = 1;
	pile_display->is_dirty = 1;
	return (count);
}

int	_pile_display_return_pdisplay(
	struct s_pile_display *pile_display,
	struct s_pdisplay *hand
)
{
	unsigned int count = 0;

	while (pile_display->cards->count)
	{
		struct s_cdll_node *node;
		struct s_card_plane *plane;
		struct s_card_desc card;
		node = cdll_pop_front(pile_display->cards);
		if (!node)
			return (0); // TODO: figure out what the fuck to do here
		plane = node->data;
		if (!plane)
			return (0);
		card = plane->card_desc;
		card_plane_destroy(plane);
		free(node);
		if (pdisplay_add_card(hand, card))
			return (dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n"), 0);
		count++;
	}
	hand->pdisplay_dirty = 1;
	pile_display->is_dirty = 1;
	return (count);
}

int	pile_display_return_to_hand(
	struct s_pile_display *pile_display,
	int is_pdisplay,
	void *hand
)
{
	if (!pile_display || !hand)
		return (0);
	switch (is_pdisplay)
	{
		case (true):
		return (_pile_display_return_pdisplay(pile_display, hand));
		case (false):
		return (_pile_display_return_to_hand(pile_display, hand));
		default:
		dprintf(STDERR_FILENO, "Neither a hand or pdisplay\n");
		return (0);
	}
	return (0);
}
