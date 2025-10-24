
#ifndef DECK_DISPLAY_ANON_H
# define DECK_DISPLAY_ANON_H

# include "cards.h"
# include "card_plane.h"
# include <notcurses/notcurses.h>

struct s_deck_display_anon
{
	struct ncplane	*parent;
	struct ncplane	*plane;
	struct ncplane	*visual;

	t_u8			hidden;
	t_u8			is_dirty;

	unsigned int	card_count;
	unsigned int	x;
	unsigned int	y;
};

int		deck_display_anon_anon_init(
		struct s_deck_display_anon **deck,
		struct ncplane *parent,
		unsigned int x,
		unsigned int y,
		unsigned int card_count
);

void	deck_display_anon_destroy(
		struct s_deck_display_anon *deck
);

int		deck_display_anon_render(
		struct notcurses *nc,
		struct s_deck_display_anon *deck
);

int		deck_display_anon_clear_screen(
		struct s_deck_display_anon *deck
);

int		deck_display_anon_inc(
		struct s_deck_display_anon *deck
);

int		deck_display_anon_dec(
		struct s_deck_display_anon *deck
);

int		deck_display_anon_add(
		struct s_deck_display_anon *deck,
		unsigned int add
);

int		deck_display_anon_sub(
		struct s_deck_display_anon *deck,
		unsigned int sub
);

int		deck_display_anon_move(
		struct s_deck_display_anon *deck,
		unsigned int x,
		unsigned int y
);

static inline int	deck_anon_is_empty(struct s_deck_display_anon *deck)
{
	if (!deck)
		return (1);
	return (
		deck->card_count == 0
	);
}

#endif