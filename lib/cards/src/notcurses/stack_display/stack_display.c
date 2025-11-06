/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_display.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:40:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:53:22 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "notcurses/stack_display.h"

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
)
{
	struct s_stack_display	*stack;
	unsigned int			card_width;
	unsigned int			card_height;

	if (!out_stack || !parent || !container || !card_set)
		return (RETURN_ERROR);
	stack = ft_calloc(1, sizeof(struct s_stack_display));
	if (!stack)
		return (RETURN_FATAL);
	
	// Get card dimensions based on orientation
	card_width = g_card_plane_width[card_orientation];
	card_height = g_card_plane_height[card_orientation];
	
	// Create the plane with size based on orientation and max cards
	// Add extra space for overflow indicators (5 chars on each side)
	stack->plane = ncplane_create(parent, &(struct ncplane_options){
		.y = y,
		.x = x,
		.rows = card_height + (orientation == STACK_ORIENTATION_VERTICAL 
			? (max_displayable > 0 ? (max_displayable - 1) * padding : 0) + 1
			: 0),
		.cols = card_width + (orientation == STACK_ORIENTATION_HORIZONTAL
			? (max_displayable > 0 ? (max_displayable - 1) * padding : 0)
			: 0) + 10,
		.userptr = NULL,
		.name = "stack_display",
		.resizecb = NULL,
		.flags = 0,
		.margin_b = 0,
		.margin_r = 0,
	});
	if (!stack->plane)
		return (free(stack), RETURN_FATAL);
	
	stack->parent = parent;
	stack->container = container;
	stack->card_set = card_set;
	stack->card_planes = NULL;
	stack->card_plane_count = 0;
	stack->orientation = orientation;
	stack->card_orientation = card_orientation;
	stack->max_displayable = max_displayable;
	stack->x = x;
	stack->y = y;
	stack->facedown = facedown;
	stack->padding = padding;
	stack->selected_index = -1;
	stack->visible = 1;
	stack->is_dirty = 1;
	stack->selected_card = NULL;
	
	*out_stack = stack;
	return (RETURN_OK);
}

void	stack_display_destroy(
	struct s_stack_display *stack
)
{
	size_t	idx;

	if (!stack)
		return ;
	
	// Destroy all card planes
	if (stack->card_planes)
	{
		idx = 0;
		while (idx < stack->card_plane_count)
		{
			if (stack->card_planes[idx])
				card_plane_destroy(stack->card_planes[idx]);
			idx++;
		}
		free(stack->card_planes);
	}
	
	// Destroy the plane
	if (stack->plane)
		ncplane_destroy(stack->plane);
	
	free(stack);
}
