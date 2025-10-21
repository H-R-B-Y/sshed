
#ifndef PDISPLAY_H
# define PDISPLAY_H

#include "card_plane.h"

/*
PDisplay is responsible for displaying the hands of other players
(non-player hands) aroudn the table.

at most 3 can be displayed at once, one on each side of the player.

Each PDisplay will have a plane associated with it, and each PDisplay
will have a set of cards in it, we dont need to know the actual cards,
(but we do for a local game i guess) but we need to know how many cards
are in the hand, and how many cards are in the shed (both face up and face down)
*/

/// @brief If we are left or right, the width is the height of the card with margin, else its the full width of the parent
# define PDISPLAY_AREA_WIDTH(orientation, parent_width, parent_height) \
	((orientation == PDISPLAY_ORIENTATION_LEFT || orientation == PDISPLAY_ORIENTATION_RIGHT) ? \
		(CARD_H_WIDTH + 3) : (parent_width))

/// @brief If we are top or bottom, the height is the height of the card with margin, else its the full height of the parent
# define PDISPLAY_AREA_HEIGHT(orientation, parent_width, parent_height) \
	((orientation == PDISPLAY_ORIENTATION_LEFT || orientation == PDISPLAY_ORIENTATION_RIGHT) ? \
		(parent_height) : (CARD_HEIGHT + 2))

/// @brief X position based on orientation
/// If left, x = 0
/// If right x = parent_width - card height - 1
/// If top then x = 0
# define PDISPLAY_AREA_X(orientation, parent_width, parent_height) \
	((orientation == PDISPLAY_ORIENTATION_LEFT) ? 0 : \
	(orientation == PDISPLAY_ORIENTATION_RIGHT) ? (parent_width - (CARD_H_WIDTH + 3)) : 0)

/// @brief Y position is just 0 because all the planes are aligned to the top
# define PDISPLAY_AREA_Y(orientation, parent_width, parent_height) 0

enum e_pdisplay_orientation
{
	PDISPLAY_ORIENTATION_LEFT,
	PDISPLAY_ORIENTATION_RIGHT,
	PDISPLAY_ORIENTATION_TOP,
	PDISPLAY_ORIENTATION_COUNT
};

enum e_pdisplay_status
{
	PDISPLAY_HAND,
	PDISPLAY_SHED,
	PDISPLAY_TYPE
};

static const char *pdisplay_orientation_str[PDISPLAY_ORIENTATION_COUNT]
__attribute__((unused)) = {
	[PDISPLAY_ORIENTATION_LEFT] = "PDISPLAY_LEFT",
	[PDISPLAY_ORIENTATION_RIGHT] = "PDISPLAY_RIGHT",
	[PDISPLAY_ORIENTATION_TOP] = "PDISPLAY_TOP",
};

struct s_pdisplay
{
	enum e_pdisplay_status		status;
	enum e_pdisplay_orientation	orientation;
	struct ncplane				*plane;
	t_u8						card_count;
	t_u8						shed_count;
	// list of card descriptors, for online we can just fill this with ace of spades
	// but for local games we need to know the actual cards.
	t_list						*cards;
	struct s_card_plane			*shed[6];
	t_u8						pdisplay_dirty;
};

int		pdisplay_create(
	struct s_pdisplay **pdisplay,
	struct ncplane *parent,
	enum e_pdisplay_orientation orientation
);

void	pdisplay_destroy(
	struct s_pdisplay *pdisplay
);

int		pdisplay_render(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
);

int		pdisplay_clear_screen(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
);

int pdisplay_show_hand(struct notcurses *nc, struct s_pdisplay *pdisplay);
int pdisplay_show_shed(struct notcurses *nc, struct s_pdisplay *pdisplay);
int pdisplay_toggle_display(struct notcurses *nc, struct s_pdisplay *pdisplay);

/*
Adding and removing cards from the pdisplay 
these are the generics for a local game where we know the actual cards
*/

int	_pdisplay_add_card(
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

int	pdisplay_add_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

int	_pdisplay_remove_card(
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

int	pdisplay_remove_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

int	_pdisplay_add_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

int	pdisplay_add_card_shed(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

int	_pdisplay_remove_card_shed(
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

int	pdisplay_remove_card_shed(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay,
	t_card_desc *card_desc
);

/*
Then we have the generics for an online game where we dont know the actual cards
*/

int	_pdisplay_add_unknown_card(
	struct s_pdisplay *pdisplay
);

int	pdisplay_add_unknown_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
);

int	_pdisplay_remove_unknown_card(
	struct s_pdisplay *pdisplay
);

int	pdisplay_remove_unknown_card(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
);

int	_pdisplay_add_unknown_card_shed(
	struct s_pdisplay *pdisplay
);

int	pdisplay_add_unknown_card_shed(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
);

int	_pdisplay_remove_unknown_card_shed(
	struct s_pdisplay *pdisplay
);

int	pdisplay_remove_unknown_card_shed(
	struct notcurses *nc,
	struct s_pdisplay *pdisplay
);

#endif
