#include "pdisplay.h"

int	_pdisplay_add_unknown_card(
	struct s_pdisplay *pdisplay
)
{
	return (_pdisplay_add_card(pdisplay, DEFAULT_ANON_CARD));
}

int	pdisplay_add_unknown_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
)
{
	return (pdisplay_add_card(nc, pdisplay, DEFAULT_ANON_CARD));
}

int	_pdisplay_remove_unknown_card(
	struct s_pdisplay *pdisplay
)
{
	return (_pdisplay_remove_card(pdisplay, DEFAULT_ANON_CARD));
}

int	pdisplay_remove_unknown_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
)
{
	return (pdisplay_remove_card(nc, pdisplay, DEFAULT_ANON_CARD));
}
