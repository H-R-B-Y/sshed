
#ifndef DECK_DISPLAY_H
# define DECK_DISPLAY_H

# include "deck.h"
# include "card_plane.h"
# include <notcurses/notcurses.h>

struct s_pdisplay;
struct s_hand;

struct s_deck_display
{
	struct ncplane	*plane;
	struct ncplane	*visual;
	unsigned int	x;
	unsigned int	y;
	
	struct s_deck	*deck;
	
	int				hidden;
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
	struct s_deck_display *deck_display
);

int			deck_display_hide(
	struct s_deck_display *deck_display
);

int			deck_display_show(
	struct s_deck_display *deck_display
);

int			deck_display_clear_screen(
	struct s_deck_display *deck_display
);

int			deck_display_toggle_visibility(
	struct s_deck_display *deck_display
);

int			deck_display_move(
	struct s_deck_display *deck_display,
	unsigned int new_x,
	unsigned int new_y
);

int	deck_display_draw_top_card(
	struct s_deck_display *deck_display,
	struct s_card_desc	*card
);

int	deck_display_peek_top_card(
	struct s_deck_display *deck_display,
	struct s_card_desc *card
);

int			return_card_to_deck_display(
	struct s_deck_display *deck_display,
	t_card_desc card_desc
);

/**
 * @returns the number of cards drawn to hand
 */
int	deck_display_draw_to_hand(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *hand,
	unsigned int count
);

int	deck_display_draw_to_hand(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *hand,
	unsigned int count
);
int deck_display_draw_to_shed(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *shed,
	unsigned int count
);


/**
 * @returns the number os cards drawn to shed
 */
int deck_display_draw_to_shed(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *shed,
	unsigned int count
);

static inline int	deck_display_is_empty(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (1);
	return (deck_display->deck->remaining == 0);
}

static inline unsigned int	deck_display_remaining_cards(
	struct s_deck_display *deck_display
)
{
	if (!deck_display || !deck_display->deck)
		return (0);
	return (deck_display->deck->remaining);
}

#endif
