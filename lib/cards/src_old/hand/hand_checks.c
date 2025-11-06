
#include "hand.h"

int hand_card_in_shed(
	struct s_hand *hand,
	struct s_card_desc card_desc)
{
	if (!hand)
		return (0);
	if (!card_is_valid(card_desc))
		return (0);
	return (card_in_array(card_desc, hand->shed, 6));
}

int hand_card_in_hand(
	struct s_hand *hand,
	struct s_card_desc card_desc)
{
	if (!hand)
		return (0);
	if (!card_is_valid(card_desc))
		return (0);
	return (card_in_list(card_desc, hand->cards));
}

int hand_contains_card(
	struct s_hand *hand,
	struct s_card_desc card_desc)
{
	return (
		hand_card_in_hand(hand, card_desc) || hand_card_in_shed(hand, card_desc));
}