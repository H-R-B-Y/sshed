#include "pdisplay.h"

# define DEFAULT_ANON_CARD \
	(struct s_card_desc){ .suit = SUIT_COUNT, .rank = RANK_COUNT }

t_card_desc *DEFAULT_ANON_CARD_PTR;

/*
There has to be a better way to do this, but for now it is 
going to be a'okay.
*/

__attribute__((constructor))
void	init_default_anon_card_ptr(void)
{
	DEFAULT_ANON_CARD_PTR = malloc(sizeof(t_card_desc));
	if (DEFAULT_ANON_CARD_PTR)
		*DEFAULT_ANON_CARD_PTR = DEFAULT_ANON_CARD;
}

__attribute__((destructor))
void	destroy_default_anon_card_ptr(void)
{
	if (DEFAULT_ANON_CARD_PTR)
	{
		free(DEFAULT_ANON_CARD_PTR);
		DEFAULT_ANON_CARD_PTR = NULL;
	}
}

int	_pdisplay_add_unknown_card(
	struct s_pdisplay *pdisplay
)
{
	return (_pdisplay_add_card(pdisplay, DEFAULT_ANON_CARD_PTR));
}

int	pdisplay_add_unknown_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
)
{
	return (pdisplay_add_card(nc, pdisplay, DEFAULT_ANON_CARD_PTR));
}

int	_pdisplay_remove_unknown_card(
	struct s_pdisplay *pdisplay
)
{
	return (_pdisplay_remove_card(pdisplay, DEFAULT_ANON_CARD_PTR));
}

int	pdisplay_remove_unknown_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
)
{
	return (pdisplay_remove_card(nc, pdisplay, DEFAULT_ANON_CARD_PTR));
}
