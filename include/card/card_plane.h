
#ifndef CARD_PLANE_H
# define CARD_PLANE_H

# include "cards.h"
# include <notcurses/notcurses.h>

// TODO: This only works for the vertical cards but we cant just swap the values for horizontal cards because the size is in characters not pixels.
# define CARD_WIDTH card_dimensions[0]
# define CARD_HEIGHT card_dimensions[1]
# define CARD_H_WIDTH card_dimensions[2]
# define CARD_H_HEIGHT card_dimensions[3]

# define CARD_PADDING 1

# define DEFAULT_PLANE_OPTS(orientation)\
	{ \
		.y = 0, \
		.x = 0, \
		.rows = (orientation == CARD_ORIENTATION_VERTICAL) ? CARD_HEIGHT : CARD_H_HEIGHT, \
		.cols = (orientation == CARD_ORIENTATION_VERTICAL) ? CARD_WIDTH : CARD_H_WIDTH, \
		.flags = 0, \
	}

# define CARD_PLANE_ALLOCATOR_SIZE \
	sizeof(struct s_card_plane) * 54 \
	+ sizeof(t_list *) * 54 \
	+ sizeof(t_free_list_node) * 54


extern t_free_list	*card_plane_allocator;

enum e_card_orientation
{
	CARD_ORIENTATION_VERTICAL,
	CARD_ORIENTATION_HORIZONTAL,
	CARD_ORIENTATION_COUNT
};

struct s_card_plane
{
	struct ncplane			*parent;
	struct ncplane			*plane;
	t_card_desc				card_desc;
	
	int						plane_shown;
	int						is_face_down;

	enum e_card_orientation	orientation;
	enum e_card_orientation	last_orientation;
};

struct s_card_plane *card_plane_create(
	struct ncplane *parent,
	t_card_desc	card_desc
);

void	card_plane_destroy(
	struct s_card_plane *card_plane
);

int	hide_card_plane(
	struct s_card_plane *card_plane
);

int	show_card_plane(
	struct s_card_plane *card_plane
);

int	redisplay_card(
	struct s_card_plane *card_plane
);

int	flip_card_plane(
	struct s_card_plane *card_plane
);

int	rotate_card_plane(
	struct s_card_plane *card_plane,
	enum e_card_orientation	new_orientation
);

#endif
