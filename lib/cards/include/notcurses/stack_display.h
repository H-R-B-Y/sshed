/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_display.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 10:39:19 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:55:04 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_DISPLAY_H
# define STACK_DISPLAY_H

# include "notcurses/card_plane.h"

enum s_stack_orientation
{
	/// @brief Vertical stack (cards move up visually)
	STACK_ORIENTATION_VERTICAL,
	/// @brief Horizontal stack (cards move right visually)
	STACK_ORIENTATION_HORIZONTAL,
	STACK_ORIENTATION_COUNT
};

/**
 * @brief A stack display of cards using ncplanes
 * 
 * Usage pattern:
 * 1. Create: stack_display_create(...)
 * 2. Sync when container changes: stack_display_sync() - creates/destroys card planes
 * 3. Render every frame: stack_display_render() - just positions existing planes
 * 
 * Example:
 *   // When adding/removing cards from container
 *   container_add_card(container, card);
 *   stack_display_sync(display);  // or just mark_dirty() and sync before render
 * 
 *   // In render loop
 *   stack_display_render(display);  // Fast, no allocation
 */
struct s_stack_display
{
	/// @brief Parent ncplane for the stack
	struct ncplane			*parent;
	/// @brief ncplane for the stack
	struct ncplane			*plane;
	/// @brief Array of card planes in the stack
	struct s_card_plane		**card_planes;
	/// @brief Number of cards in the stack
	size_t					card_plane_count;
	/// @brief Card container backing the stack
	struct s_container		*container;
	/// @brief Card set for card visuals
	struct s_card_set		*card_set;

	/// @brief Orientation of the stack
	enum s_stack_orientation	orientation;
	/// @brief Orientation of the cards
	enum e_card_orientation	card_orientation;
	/// @brief Maximum number of displayable cards
	t_u8			max_displayable;
	/// @brief Current x position of the stack
	unsigned int	x;
	/// @brief Current y position of the stack
	unsigned int	y;
	/// @brief Are cards facedown
	t_u8			facedown;
	/// @brief Padding between cards
	unsigned int	padding;
	/// @brief Currently selected card index
	ssize_t			selected_index;
	/// @brief Is the stack visible
	t_u8			visible;
	/// @brief Is the stack dirty and needs re-rendering
	t_u8			is_dirty;
	/// @brief Pointer to the currently selected card
	struct s_card_desc	*selected_card;
};

t_returncode	stack_display_create(
	struct s_stack_display **out_stack,
	struct ncplane *parent,
	struct s_container *container,
	struct s_card_set *card_set,
	enum s_stack_orientation orientation,
	enum e_card_orientation card_orientation,
	t_u8 max_displayable,
	unsigned int x,
	unsigned int y,
	t_u8 facedown,
	unsigned int padding
);

void			stack_display_destroy(
	struct s_stack_display *stack
);

t_returncode	stack_display_sync(
	struct s_stack_display *stack
);

t_returncode	stack_display_render(
	struct s_stack_display *stack
);

t_returncode	stack_display_mark_dirty(
	struct s_stack_display *stack
);

t_returncode	stack_display_set_position(
	struct s_stack_display *stack,
	unsigned int x,
	unsigned int y
);

t_returncode	stack_display_erase(
	struct s_stack_display *stack
);

t_returncode	stack_display_set_facedown(
	struct s_stack_display *stack,
	t_u8 facedown
);

t_returncode	stack_display_set_visible(
	struct s_stack_display *stack,
	t_u8 visible
);

t_returncode	stack_display_set_selected_card(
	struct s_stack_display *stack,
	ssize_t selected_index
);

t_returncode	stack_display_select_next_card(
	struct s_stack_display *stack
);

t_returncode	stack_display_select_previous_card(
	struct s_stack_display *stack
);

#endif
