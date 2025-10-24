
#ifndef PILE_DISPLAY_H
# define PILE_DISPLAY_H

# include <notcurses/notcurses.h>
# include "card_plane.h"

/*
So a pile represents a stack of cards
they can be face up or face down, and display in a stacked manner.
We want all of these variables to be changeable at runtime so we need
to factor that into how we design this.
Since we can only display one visual per ncplane, we will need to create
a ncplane for each card up to max_stack
*/

enum e_pile_display_orientation
{
	PILE_DISPLAY_HORIZONTAL,
	PILE_DISPLAY_VERTICAL,
	PILE_DISPLAY_ORIENTATION_COUNT
};

/*
Or would it be better to use cdll of card_planes?
card_planes we can already manipulate the orientation, and
we can just hide them when not in use.
Yeah this seems a better idea.
*/
struct s_pile_display
{
	enum e_pile_display_orientation		orientation;
	enum e_card_orientation				card_orientation;
	unsigned int						x;
	unsigned int						y;

	unsigned int						padding;
	struct ncplane						*plane;

	t_u8								is_face_down;
	t_cdll								*cards; // circular doubly linked list of card_planes
	unsigned int						max_stack;

	int									is_visible;
	t_u8								is_dirty;
};

int		pile_display_create(
	struct s_pile_display **pile_display,
	struct ncplane *parent,
	unsigned int padding,
	enum e_card_orientation card_orientation,
	enum e_pile_display_orientation orientation,
	unsigned int max_stack,
	unsigned int x,
	unsigned int y
);

void	pile_display_destroy(
	struct s_pile_display *pile_display
);

int		pile_display_render(
	struct notcurses *nc,
	struct s_pile_display *pile_display
);

int		pile_display_add_card_top(
	struct notcurses *nc,
	struct s_pile_display *pile_display,
	t_card_desc card_desc
);

int		pile_display_add_card_bottom(
	struct notcurses *nc,
	struct s_pile_display *pile_display,
	t_card_desc card_desc
);

int		pile_display_remove_card(
	struct notcurses *nc,
	struct s_pile_display *pile_display,
	t_card_desc card_desc
);

int pile_display_clear_screen(
	struct notcurses *nc,
	struct s_pile_display *pile_display
);

int pile_display_toggle_visibility(
	struct notcurses *nc,
	struct s_pile_display *pile_display
);

int pile_display_move(
	struct s_pile_display *pile_display,
	unsigned int new_x,
	unsigned int new_y
);

// TODO: implement the following
// int pile_display_set_face_down(
// 	struct s_pile_display *pile_display,
// 	t_u8 is_face_down
// );

// int pile_display_set_padding(
// 	struct s_pile_display *pile_display,
// 	unsigned int padding
// );

// int pile_display_set_max_stack(
// 	struct s_pile_display *pile_display,
// 	unsigned int max_stack
// );

// int pile_display_set_card_orientation(
// 	struct s_pile_display *pile_display,
// 	enum e_card_orientation card_orientation
// );

inline int	pile_display_is_empty(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return (1);
	return (pile_display->cards->count == 0);
}

inline unsigned int	pile_display_get_count(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return (-1);
	return ((unsigned int)(pile_display->cards->count));
}

#endif
