
#ifndef HAND_H
# define HAND_H

#include "cards.h"

# include <notcurses/notcurses.h>

# define CARD_WIDTH 8
# define CARD_HEIGHT 5

# define DEFAULT_PLANE_OPTS \
	{ \
		.y = 0, \
		.x = 0, \
		.rows = CARD_HEIGHT, /* Assuming card height is 7 */ \
		.cols = CARD_WIDTH, /* Assuming card width is 9 */ \
		.flags = 0, \
	}

# define HAND_ARENA_ALLOCATOR_SIZE 54 * (sizeof(struct s_card_plane) + sizeof(t_list) + sizeof(t_free_list_node)) \
		+ 6 * (sizeof(struct s_card_plane) + sizeof(t_list) + sizeof(t_free_list_node))

/*
Whats the best way to do this then, 
each card will have it's own plane because it needs one for sixel graphics
and hand will need it's own plane to contain the cards.
do we make a new struct to store the card desc and the plane together?

*/
enum e_hand_status
{
	HAND_DISPLAY_HAND,
	HAND_DISPLAY_SHED,
	HAND_DISPLAY_TYPE
};

struct s_card_plane
{
	t_card_desc		*card_desc;
	int				plane_shown;
	int				is_face_down;
	struct ncplane	*plane;
};

struct s_hand
{
	enum e_hand_status	status;
	enum e_hand_status	last_status;
	int					hand_dirty;
	t_free_list			allocator; // for card planes
	t_list				*cards; // contains struct s_card_plane
	int					card_count;
	int					card_selected[2]; // selected hand and shed

	/*

	Shed will have 6 cards at the start of the game,
	the first 3 cards are face down, the last are face up.

	*/
	struct s_card_plane *shed[6];
	t_u8				shed_count;
	struct ncplane		*hand_plane;
};

int		hand_create(struct s_hand **hand, struct ncplane *parent, int max_width, int y, int x);
void	hand_destroy(struct s_hand *hand);

/*
Functions for adding to the hand.
Those marked with an underscore do not re-render the hand after modification.
*/
int		_hand_add_card(struct s_hand *hand, t_card_desc *card_desc);
int		hand_add_card(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc);

void	_hand_remove_card(struct s_hand *hand, t_card_desc *card_desc);
void	hand_remove_card(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc);

/*
Functions for adding to the shed.
Those marked with an underscore do not re-render the hand after modification.
*/
int		_hand_add_card_to_shed(struct s_hand *hand, t_card_desc *card_desc);
// First 3 cards are face down, last 3 are face up
int		hand_add_card_to_shed(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc);

void	_hand_remove_card_shed(struct s_hand *hand, t_card_desc *card_desc);
void	hand_remove_card_shed(struct notcurses *nc, struct s_hand *hand, t_card_desc *card_desc);

void	hand_render(struct notcurses *nc, struct s_hand *hand);
void	hand_clear_screen(struct notcurses *nc, struct s_hand *hand);

void	hand_show_hand(struct notcurses *nc, struct s_hand *hand);
void	hand_show_shed(struct notcurses *nc, struct s_hand *hand);
void	hand_toggle_display(struct notcurses *nc, struct s_hand *hand);


/*
Card plane fncs
*/

struct s_card_plane	*card_plane_create(struct s_hand *hand, t_card_desc *card_desc);
void				card_plane_destroy(struct s_card_plane *card_plane);
int					hide_card(struct notcurses *nc, struct s_hand *hand, struct s_card_plane *card_plane);
int					show_card(struct notcurses *nc, struct s_hand *hand, struct s_card_plane *card_plane);
int					redisplay_card(struct notcurses *nc, struct s_hand *hand, struct s_card_plane *card_plane);

void	debug_print_shed(struct s_hand *hand);
#endif
