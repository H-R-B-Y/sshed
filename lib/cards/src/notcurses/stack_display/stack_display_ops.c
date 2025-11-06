/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_display_ops.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:42:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:35:53 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "notcurses/stack_display.h"

t_returncode	stack_display_set_position(
	struct s_stack_display *stack,
	unsigned int x,
	unsigned int y
)
{
	if (!stack || !stack->plane)
		return (RETURN_ERROR);
	
	if (ncplane_move_yx(stack->plane, y, x) < 0)
		return (RETURN_ERROR);
	
	stack->x = x;
	stack->y = y;
	stack->is_dirty = 1;
	
	return (RETURN_OK);
}

t_returncode	stack_display_erase(
	struct s_stack_display *stack
)
{
	size_t	idx;

	if (!stack)
		return (RETURN_ERROR);
	
	// Erase the main plane
	if (stack->plane)
		ncplane_erase(stack->plane);
	
	// Hide all card planes
	idx = 0;
	while (idx < stack->card_plane_count)
	{
		if (stack->card_planes[idx])
			card_plane_set_visible(stack->card_planes[idx], 0);
		idx++;
	}
	
	return (RETURN_OK);
}

t_returncode	stack_display_set_facedown(
	struct s_stack_display *stack,
	t_u8 facedown
)
{
	size_t	idx;

	if (!stack)
		return (RETURN_ERROR);
	
	stack->facedown = facedown;
	
	// Update all existing card planes
	idx = 0;
	while (idx < stack->card_plane_count)
	{
		if (stack->card_planes[idx])
			card_plane_set_face_down(stack->card_planes[idx], facedown);
		idx++;
	}
	
	stack->is_dirty = 1;
	return (RETURN_OK);
}

t_returncode	stack_display_set_visible(
	struct s_stack_display *stack,
	t_u8 visible
)
{
	if (!stack)
		return (RETURN_ERROR);
	
	stack->visible = visible;
	stack->is_dirty = 1;
	
	// Trigger a render to show/hide the stack
	return (stack_display_render(stack));
}
