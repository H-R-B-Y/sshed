
#ifndef DECK_H
# define DECK_H

# define FT_INCLUDE_ALL
# include "libft.h"
# include "cards.h"

struct s_deck
{
	t_card_desc	*cards[55];
	int			has_jokers;
	int			remaining;
};

struct s_deck	*deck_create(int include_jokers);
void			deck_destroy(struct s_deck *deck);

int				deck_shuffle(struct s_deck *deck, unsigned int seed);

t_card_desc		*deck_draw_card(struct s_deck *deck);
t_card_desc		*deck_peek_card(struct s_deck *deck, unsigned int index);
void			deck_return_card(struct s_deck *deck, t_card_desc *card);


#endif

