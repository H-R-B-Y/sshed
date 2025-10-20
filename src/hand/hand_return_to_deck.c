
#include "hand.h"
#include "deck.h"

int	hand_return_to_deck(
	struct notcurses *nc,
	struct s_hand *hand,
	struct s_deck *deck
)
{
	if (!nc || !hand || !deck)
		return (1);
	while (hand->card_count > 0)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)hand->cards->content;
		hand_remove_card(nc, hand, card_plane->card_desc);
		deck_return_card(deck, card_plane->card_desc);
	}
	hand_render(nc, hand);
	return (0);
}
