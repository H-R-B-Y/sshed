
#include "pdisplay.h"

// Checks


int	pdisplay_card_is_shed(
	struct s_pdisplay *pdispay,
	struct s_card_desc card_desc
)
{
	if (!pdispay)
		return (0);
	if (!card_is_valid(card_desc))
		return (0);
	return (card_in_array(card_desc, pdispay->shed, 6));
}

int	pdisplay_card_is_hand(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
)
{
	if (!pdisplay)
		return (0);
	if (!card_is_valid(card_desc))
		return (0);
	return (card_in_list(card_desc, pdisplay->cards));
}


int	pdisplay_has_card(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
)
{
	if (!pdisplay)
		return (0);
	if (!card_is_valid(card_desc))
		return (0);
	return (
		pdisplay_card_is_hand(pdisplay, card_desc)
		|| pdisplay_card_is_shed(pdisplay, card_desc)
	);
}
