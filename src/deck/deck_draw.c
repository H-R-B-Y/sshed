
#include "deck.h"

t_card_desc		*deck_draw_card(struct s_deck *deck)
{
	t_card_desc	*card;

	if (deck->remaining == 0)
		return (NULL);
	deck->remaining--;
	card = deck->cards[deck->remaining];
	deck->cards[deck->remaining] = NULL;
	return (card);
}

t_card_desc		*deck_peek_card(struct s_deck *deck, unsigned int index)
{
	if (index >= (unsigned int)deck->remaining)
		return (NULL);
	return (deck->cards[index]);
}

/*
We have to be careful here because we could add multiple cards to the same deck
*/
void			deck_return_card(struct s_deck *deck, t_card_desc *card)
{
	if (deck->remaining >= (deck->has_jokers ? 54 : 52))
		return;
	deck->cards[deck->remaining] = card;
	deck->remaining++;
}
