
#ifndef CARD_PLANE_H
# define CARD_PLANE_H

# include "cards.h"
# include <notcurses/notcurses.h>

/// @brief Standard card dimensions: width, height, half-width, half-height
extern unsigned int	card_dimensions[4];
# define CARD_WIDTH card_dimensions[0]
# define CARD_HEIGHT card_dimensions[1]
# define CARD_H_WIDTH card_dimensions[2]
# define CARD_H_HEIGHT card_dimensions[3]

/// @brief Padding around cards (not really used currently)
# define CARD_PADDING 1

/// @brief Default ncplane options for card planes
# define DEFAULT_PLANE_OPTS(orientation)\
	{ \
		.y = 0, \
		.x = 0, \
		.rows = (orientation == CARD_ORIENTATION_VERTICAL) ? CARD_HEIGHT : CARD_H_HEIGHT, \
		.cols = (orientation == CARD_ORIENTATION_VERTICAL) ? CARD_WIDTH : CARD_H_WIDTH, \
		.flags = 0, \
	}

// TODO: We really should have all 4 orientations here for future use with non-symmetrical cards
enum e_card_orientation
{
	/// @brief Vertical orientation (tall)
	CARD_ORIENTATION_VERTICAL,
	/// @brief Horizontal orientation (wide)
	CARD_ORIENTATION_HORIZONTAL,
	/// @brief Count of card orientations
	CARD_ORIENTATION_COUNT
};

struct s_card_plane
{
	/// @brief Parent ncplane
	struct ncplane			*parent;
	/// @brief The ncplane representing the card
	struct ncplane			*plane;
	/// @brief The card description (rank and suit)
	t_card_desc				card_desc;
	/// @brief Whether the card is currently shown on screen
	int						plane_shown;
	/// @brief Whether the card is face down
	int						is_face_down;
	/// @brief Current orientation of the card
	enum e_card_orientation	orientation;
	/// @brief Last orientation of the card (for rotation tracking)
	enum e_card_orientation	last_orientation;
};

/**
 * @brief Create a new card plane
 *
 * @param parent The parent ncplane
 * @param card_desc The card description (rank and suit)
 * @return A pointer to the new card plane, or NULL on failure
 */
struct s_card_plane *card_plane_create(
	struct ncplane *parent,
	t_card_desc	card_desc
);

/**
 * @brief Destroy a card plane
 *
 * @param card_plane The card plane to destroy
 * @return 0 on success, 1 on error
 */
void	card_plane_destroy(
	struct s_card_plane *card_plane
);

/**
 * @brief Hide a card plane
 *
 * @param card_plane The card plane to hide
 * @return 0 on success, 1 on error
 */
int	hide_card_plane(
	struct s_card_plane *card_plane
);

/**
 * @brief Show a card plane
 *
 * @param card_plane The card plane to show
 * @return 0 on success, 1 on error
 */
int	show_card_plane(
	struct s_card_plane *card_plane
);

/**
 * @brief Redraw a card plane
 *
 * @param card_plane The card plane to redraw
 * @return 0 on success, 1 on error
 */
int	redisplay_card(
	struct s_card_plane *card_plane
);

/**
 * @brief Flip a card plane (face up/down)
 *
 * @param card_plane The card plane to flip
 * @return 0 on success, 1 on error
 */
int	flip_card_plane(
	struct s_card_plane *card_plane
);

/**
 * @brief Toggle the face up/down state of a card plane
 *
 * @param card_plane The card plane to toggle
 * @return 0 on success, 1 on error
 */
int	rotate_card_plane(
	struct s_card_plane *card_plane,
	enum e_card_orientation	new_orientation
);

/**
 * @brief Check if a card description is in a linked list of card planes
 * 
 * TODO: Move this somewhere that makes more sense
 * 
 * @param card_desc The card description to check
 * @param list The linked list of card planes
 * @return 1 if the card is in the list, 0 otherwise
 */
int	card_in_list(
	struct s_card_desc card_desc,
	t_list *list
);

/**
 * @brief Check if a card description is in an array of card planes
 *
 * TODO: Move this somewhere that makes more sense
 * 
 * @param card_desc The card description to check
 * @param planes The array of card planes
 * @param card_count The number of cards in the array
 * @return 1 if the card is in the array, 0 otherwise
 */
int	card_in_array(
	struct s_card_desc card_desc,
	struct s_card_plane **planes,
	size_t card_count
);

int make_plane_transparent(struct ncplane *plane);

#endif
