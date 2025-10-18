
#ifndef HAND_H
# define HAND_H

#include "cards.h"

# include <notcurses/notcurses.h>

/*
Whats the best way to do this then, 
each card will have it's own plane because it needs one for sixel graphics
and hand will need it's own plane to contain the cards.
do we make a new struct to store the card desc and the plane together?

*/

struct s_card_plane
{
	t_card_desc		*card_desc;
	struct ncplane	*plane;
};

struct s_hand
{
	int				hand_dirty;
	t_free_list		allocator; // for card planes
	t_list			*cards; // contains struct s_card_plane
	int				card_count;
	int				card_selected;
	struct ncplane	*hand_plane;
};

int		hand_create(struct s_hand **hand, struct ncplane *parent, int max_width, int y, int x);
void	hand_destroy(struct s_hand *hand);

int		hand_add_card(struct s_hand *hand, t_card_desc *card_desc);
void	hand_remove_card(struct s_hand *hand, t_card_desc *card_desc);
void	hand_render(struct notcurses *nc, struct s_hand *hand);

#endif
