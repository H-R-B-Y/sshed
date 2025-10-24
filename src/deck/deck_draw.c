
#include "deck.h"

int	deck_draw_card(struct s_deck *deck, struct s_card_desc *card)
{
	if (deck->remaining == 0)
		return (1);
	deck->remaining--;
	(*card) = (*deck->cards[deck->remaining]);
	return (0);
}

int	deck_peek_card(struct s_deck *deck, unsigned int index, struct s_card_desc *card)
{
	if (index >= (unsigned int)deck->remaining)
		return (1);
	(*card) = (*deck->cards[index]);
	return (0);
}

/*
We have to be careful here because we could add multiple cards to the same deck
*/
void	deck_return_card(struct s_deck *deck, struct s_card_desc card)
{
	if (deck->remaining >= (deck->has_jokers ? 54 : 52))
		return;
	(*deck->cards[deck->remaining]) = card;
	deck->remaining++;
}
