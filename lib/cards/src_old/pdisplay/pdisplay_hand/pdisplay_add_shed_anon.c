#include "pdisplay.h"

int	_pdisplay_add_unknown_card_shed(
	struct s_pdisplay *pdisplay
)
{
	if (!pdisplay)
		return (1);
	return _pdisplay_add_card_shed(pdisplay, DEFAULT_ANON_CARD);
}

int	pdisplay_add_unknown_card_shed(
	struct s_pdisplay *pdisplay
)
{
	return (pdisplay_add_card_shed(pdisplay, DEFAULT_ANON_CARD));
}

int	_pdisplay_remove_unknown_card_shed(
	struct s_pdisplay *pdisplay
)
{
	if (!pdisplay)
		return (1);
	return _pdisplay_remove_card_shed(pdisplay, DEFAULT_ANON_CARD);
}

int	pdisplay_remove_unknown_card_shed(
	struct s_pdisplay *pdisplay
)
{
	return (pdisplay_remove_card_shed(pdisplay, DEFAULT_ANON_CARD));
}