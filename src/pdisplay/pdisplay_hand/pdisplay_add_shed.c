#include "pdisplay.h"

int	_pdisplay_add_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
)
{
	if (!pdisplay || !card_desc)
		return (1);
	if (pdisplay->shed_count >= 6)
		return (1); // Shed is full
	pdisplay->shed[pdisplay->shed_count] = card_plane_create(card_desc);
	if (!pdisplay->shed[pdisplay->shed_count])
		return (1);
	pdisplay->shed_count++;
	pdisplay->pdisplay_dirty = 1;
	return (0);
}

int	pdisplay_add_card_shed(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
)
{
	int	ret;

	ret = _pdisplay_add_card_shed(pdisplay, card_desc);
	pdisplay_render(nc, pdisplay);
	return (ret);
}

int	_pdisplay_remove_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
)
{
	if (!pdisplay || !card_desc)
		return (1);
	for (unsigned int idx = 0; idx < 6; idx++)
	{
		struct s_card_plane *card_plane = pdisplay->shed[idx];
		if (card_plane && card_plane->card_desc == card_desc)
		{
			card_plane_destroy(card_plane);
			pdisplay->shed[idx] = NULL;
			pdisplay->shed_count--;
			pdisplay->pdisplay_dirty = 1;
			return (0);
		}
	}
	return (1);
}

int	pdisplay_remove_card_shed(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
)
{
	int	ret;
	
	ret = _pdisplay_remove_card_shed(pdisplay, card_desc);
	pdisplay_render(nc, pdisplay);
	return (ret);
}

