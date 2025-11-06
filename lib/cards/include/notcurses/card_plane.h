/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_plane.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 17:10:59 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:23:35 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CARD_PLANE_H
# define CARD_PLANE_H

# include "core/card.h"
# include "core/card_set.h"
# include "core/container.h"
# include <notcurses/notcurses.h>

# define CARD_PLANE_DEFAULT_OPTIONS (struct ncvisual_options){	\
	.x = 0,\
	.y = 0,\
	.blitter = NCBLIT_PIXEL,\
	.scale = NCSCALE_NONE,\
	.flags = NCVISUAL_OPTION_CHILDPLANE|NCVISUAL_OPTION_NODEGRADE,\
}

/**
 * @brief Which way round is the card
 * 
 */
enum e_card_orientation
{
	/// @brief North / Upright
	CARD_UP,
	/// @brief South / Upside down
	CARD_DOWN,
	/// @brief West / Left
	CARD_LEFT,
	/// @brief East / Right
	CARD_RIGHT,
	CARD_ORIENTATION_COUNT
};

HEADER_STATIC_CONST char *g_card_orientation_str[CARD_ORIENTATION_COUNT] = {
	[CARD_UP] = "UP",
	[CARD_DOWN] = "DOWN",
	[CARD_LEFT] = "LEFT",
	[CARD_RIGHT] = "RIGHT",
};

/// @brief Global card plane dimensions for each orientation
extern unsigned int	g_card_plane_width[4];
/// @brief Global card plane dimensions for each orientation
extern unsigned int	g_card_plane_height[4];

/**
 * @brief Visual representation of a card
 * 
 */
struct s_card_visual
{
	/// @brief Visuals for each orientation
	struct ncvisual		*visual[CARD_ORIENTATION_COUNT];
	/// @brief Associated card set
	struct s_card_set	*card_set;
	/// @brief User data associated with the visual
	void				*user_data;
};

t_returncode card_visual_load(
	struct notcurses *nc,
	struct s_card_visual *visual,
	struct s_card_desc card,
	const char *visual_directory
);

t_returncode card_set_load_visuals(
	struct notcurses *nc,
	struct s_card_set *set,
	const char *visual_directory
);

struct s_card_visual* card_set_get_visual(
	struct s_card_set *set,
	struct s_card_desc card
);

void	card_visual_free(
	struct s_card_visual *visual
);

/**
 * @brief A card rendered on a ncplane
 * 
 */
struct s_card_plane
{
	struct ncplane			*parent;
	/// @brief Associated ncplane
	struct ncplane			*plane;
	/// @brief Associated card visual
	struct s_card_desc		card;
	/// @brief Associated card set
	struct s_card_set		*card_set;
	/// @brief Orientation of the card
	enum e_card_orientation	orientation;
	/// @brief Is the card face down
	t_u8					face_down;
	/// @brief Is the card visible
	t_u8					visible;
};

t_returncode card_plane_create(
	struct s_card_plane **out_card_plane,
	struct ncplane *parent,
	struct s_card_set *card_set,
	struct s_card_desc card,
	enum e_card_orientation orientation,
	t_u8 face_down
);

void	card_plane_destroy(
	struct s_card_plane *card_plane
);

t_returncode card_plane_set_card(
	struct s_card_plane *card_plane,
	struct s_card_set *card_set,
	struct s_card_desc card
);

t_returncode card_plane_set_orientation(
	struct s_card_plane *card_plane,
	enum e_card_orientation orientation
);

t_returncode card_plane_set_face_down(
	struct s_card_plane *card_plane,
	t_u8 face_down
);

t_returncode card_plane_set_visible(
	struct s_card_plane *card_plane,
	t_u8 visible
);

t_returncode card_plane_render(
	struct s_card_plane *card_plane
);

t_returncode card_plane_erase(
	struct s_card_plane *card_plane
);

t_returncode card_plane_move(
	struct s_card_plane *card_plane,
	int y,
	int x
);

#endif
