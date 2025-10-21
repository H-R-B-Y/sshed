
#ifndef DECK_DISPLAY_H
# define DECK_DISPLAY_H

# include <notcurses/notcurses.h>
# include "card_plane.h"
# include "deck.h"

struct s_deck_display
{
	unsigned int	x;
	unsigned int	y;
	struct ncplane	*plane;
	struct s_deck	*deck;
	int				is_visible;
	t_u8			is_dirty;
};

int			deck_display_create(
	struct s_deck_display **deck_display,
	struct ncplane *parent,
	struct s_deck *deck,
	unsigned int x,
	unsigned int y
);

void		deck_display_destroy(
	struct s_deck_display *deck_display
);

int			deck_display_render(
	struct notcurses *nc,
	struct s_deck_display *deck_display
);

int			deck_display_hide(
	struct notcurses *nc,
	struct s_deck_display *deck_display
);

int			deck_display_show(
	struct notcurses *nc,
	struct s_deck_display *deck_display
);

int			deck_display_toggle_visibility(
	struct notcurses *nc,
	struct s_deck_display *deck_display
);

int		deck_display_move(
	struct s_deck_display *deck_display,
	unsigned int new_x,
	unsigned int new_y
);

t_card_desc	*deck_display_peek_top_card(
	struct s_deck_display *deck_display
);

t_card_desc	*deck_display_draw_top_card(
	struct s_deck_display *deck_display
);

int			return_card_to_deck_display(
	struct s_deck_display *deck_display,
	t_card_desc *card_desc
);

inline int	deck_display_is_empty(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (1);
	return (deck_display->deck->remaining == 0);
}

inline unsigned int	deck_display_remaining_cards(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (0);
	return (deck_display->deck->remaining);
}

#endif
