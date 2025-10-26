
# include "hand.h"

int	hand_calculate_max_displayable(
	struct ncplane *stdplane,
	struct s_hand *hand
)
{
	unsigned int	width, height;
	/*
	So we want 
	1 card horizontally margin to the left and right of the hand
	then we want to try and fit as many cards in between that,
	accounting for the characters to represent extra cards
	*/
	ncplane_dim_yx(stdplane, &height, &width);
	hand->max_cards_to_display
	= (width - (2 * (CARD_H_WIDTH + 2))) / (CARD_WIDTH + 2) - 1;
	return (0);
}
