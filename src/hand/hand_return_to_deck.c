
#include "hand.h"
#include "deck.h"

int	hand_return_to_deck(
	struct s_hand *hand,
	struct s_deck *deck
)
{
	struct s_card_plane	*card_plane;

	if (!hand || !deck)
		return (1);
	while (hand->card_count > 0)
	{
		card_plane = (struct s_card_plane *)hand->_cards.head;
		deck_return_card(deck, card_plane->card_desc);
		hand_remove_card(hand, card_plane->card_desc);
	}
	hand_render(hand);
	return (0);
}
