
#include "pdisplay.h"

int	pdisplay_calculate_max_displayable(
	struct ncplane *stdplane,
	struct s_pdisplay *pdisplay
)
{
	unsigned int	width, height;
	/*
	bit different here as it depends on the orientation
	if its top then we can just copy how the hand does it
	but if it's left or right then we need to calc against verical cards
	*/
	if (!stdplane || !pdisplay)
		return (1);
	ncplane_dim_yx(stdplane, &height, &width);
	if (pdisplay->orientation == PDISPLAY_ORIENTATION_TOP)
	{
		pdisplay->max_cards_to_display
		= ((width - (2 * (CARD_H_WIDTH + 2))) / (CARD_WIDTH)) - 1;
		return (0);
	}
	else
	{
		pdisplay->max_cards_to_display
		= ((height - (2 * (CARD_HEIGHT + 1))) / (CARD_H_HEIGHT)) - 2;
		return (0);
	}
}
