/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_display_render.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:41:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:55:03 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "notcurses/stack_display.h"

#define min(a,b) (((a)<(b))?(a):(b))

static t_returncode	stack_display_sync_cards(
	struct s_stack_display *stack
)
{
	struct s_card_desc	*cards;
	size_t				card_count;
	size_t				idx;
	struct s_card_plane	**new_planes;

	// Get all cards from the container
	if (container_get_all_cards(stack->container, &cards, &card_count) != RETURN_OK)
		return (RETURN_ERROR);
	
	// If we need more planes, reallocate
	if (card_count > stack->card_plane_count)
	{
		new_planes = ft_realloc(stack->card_planes, 
			sizeof(struct s_card_plane *) * stack->card_plane_count,
			sizeof(struct s_card_plane *) * card_count);
		if (!new_planes)
			return (free(cards), RETURN_FATAL);
		stack->card_planes = new_planes;
		
		// Initialize new planes to NULL
		idx = stack->card_plane_count;
		while (idx < card_count)
		{
			stack->card_planes[idx] = NULL;
			idx++;
		}
	}
	
	// Create or update card planes
	idx = 0;
	while (idx < card_count)
	{
		t_u8 is_face_down;
		
		// For shed containers, face-down cards at indices >= half_capacity
		if (stack->container->type == CONTAINER_TYPE_SHED)
		{
			is_face_down = (idx >= stack->container->data.shed_container.half_capacity);
		}
		else
		{
			is_face_down = stack->facedown;
		}
		
		if (!stack->card_planes[idx])
		{
			// Create new card plane
			if (card_plane_create(
				&stack->card_planes[idx],
				stack->plane,
				stack->card_set,
				cards[idx],
				stack->card_orientation,
				is_face_down
			) != RETURN_OK)
			{
				free(cards);
				return (RETURN_ERROR);
			}
		}
		else
		{
			// Update existing card plane
			card_plane_set_card(stack->card_planes[idx], stack->card_set, cards[idx]);
			card_plane_set_orientation(stack->card_planes[idx], stack->card_orientation);
			card_plane_set_face_down(stack->card_planes[idx], is_face_down);
		}
		idx++;
	}
	
	// Destroy excess card planes if we have fewer cards now
	idx = card_count;
	while (idx < stack->card_plane_count)
	{
		if (stack->card_planes[idx])
		{
			card_plane_destroy(stack->card_planes[idx]);
			stack->card_planes[idx] = NULL;
		}
		idx++;
	}
	
	stack->card_plane_count = card_count;
	free(cards);
	return (RETURN_OK);
}

t_returncode	stack_display_sync(
	struct s_stack_display *stack
)
{
	if (!stack)
		return (RETURN_ERROR);
	return (stack_display_sync_cards(stack));
}

t_returncode	stack_display_mark_dirty(
	struct s_stack_display *stack
)
{
	if (!stack)
		return (RETURN_ERROR);
	stack->is_dirty = 1;
	return (RETURN_OK);
}

t_returncode	stack_display_render(
	struct s_stack_display *stack
)
{
	size_t				idx;
	size_t				displayable_count;
	struct s_card_plane	*prev_plane;
	unsigned int		card_width;

	if (!stack)
		return (RETURN_ERROR);
	
	// If not visible, erase and return
	if (!stack->visible)
	{
		ncplane_erase(stack->plane);
		return (RETURN_OK);
	}
	
	// If not dirty, no need to render
	if (!stack->is_dirty)
		return (RETURN_OK);
	
	// Erase the plane before rendering
	ncplane_erase(stack->plane);
	
	// Calculate how many cards to display and visible range
	displayable_count = min(stack->max_displayable, stack->card_plane_count);
	
	size_t visible_start = 0;
	size_t visible_end = displayable_count;
	
	// If there are more cards than displayable, calculate the visible window
	if (stack->card_plane_count > stack->max_displayable)
	{
		if (stack->selected_index >= 0 && stack->selected_index < (ssize_t)stack->card_plane_count)
		{
			// Center on selected card
			ssize_t half_display = stack->max_displayable / 2;
			ssize_t start = stack->selected_index - half_display;
			
			if (start < 0)
				start = 0;
			else if (start + stack->max_displayable > stack->card_plane_count)
				start = stack->card_plane_count - stack->max_displayable;
			
			visible_start = (size_t)start;
			visible_end = visible_start + stack->max_displayable;
		}
	}
	
	// Hide all cards first
	idx = 0;
	while (idx < stack->card_plane_count)
	{
		if (stack->card_planes[idx])
			card_plane_set_visible(stack->card_planes[idx], 0);
		idx++;
	}
	
	// Render visible cards
	idx = visible_start;
	prev_plane = NULL;
	size_t display_idx = 0;
	
	// Calculate offset for centering cards (space for overflow text)
	unsigned int card_offset_x = 0;
	unsigned int card_offset_y = 0;
	
	if (stack->orientation == STACK_ORIENTATION_HORIZONTAL)
		card_offset_x = 5; // Space for left overflow text like "999<"
	else
		card_offset_y = 1; // Space for top overflow text like "^999"
	
	while (idx < visible_end && idx < stack->card_plane_count)
	{
		if (stack->card_planes[idx])
		{
			// Set card plane visibility
			card_plane_set_visible(stack->card_planes[idx], 1);
			
			// Render the card
			if (card_plane_render(stack->card_planes[idx]) != RETURN_OK)
				return (RETURN_ERROR);
			
			// Position the card based on stack orientation (using display_idx for positioning)
			if (stack->orientation == STACK_ORIENTATION_VERTICAL)
			{
				card_plane_move(stack->card_planes[idx], 
					card_offset_y + display_idx * stack->padding, 
					card_offset_x);
			}
			else // STACK_ORIENTATION_HORIZONTAL
			{
				card_plane_move(stack->card_planes[idx], 
					card_offset_y, 
					card_offset_x + display_idx * stack->padding);
			}
			
			// Stack cards properly (newer cards on top)
			if (prev_plane && prev_plane->plane && stack->card_planes[idx]->plane)
				ncplane_move_below(stack->card_planes[idx]->plane, prev_plane->plane);
			
			prev_plane = stack->card_planes[idx];
			display_idx++;
		}
		idx++;
	}
	
	// Display overflow count if needed
	if (stack->card_plane_count > stack->max_displayable)
	{
		size_t		overflow_left;
		size_t		overflow_right;
		
		card_width = g_card_plane_width[stack->card_orientation];
		
		overflow_left = visible_start;
		overflow_right = stack->card_plane_count - visible_end;
		
		// Display left overflow
		if (overflow_left > 0)
		{
			if (stack->orientation == STACK_ORIENTATION_HORIZONTAL)
			{
				ncplane_printf_yx(stack->plane, 
					g_card_plane_height[stack->card_orientation] / 2, 
					0, 
					"%zu<", overflow_left);
			}
			else // VERTICAL
			{
				ncplane_printf_yx(stack->plane, 
					0, 
					g_card_plane_width[stack->card_orientation] / 2, 
					"^%zu", overflow_left);
			}
		}
		
		// Display right overflow
		if (overflow_right > 0)
		{
			if (stack->orientation == STACK_ORIENTATION_HORIZONTAL)
			{
				ncplane_printf_yx(
					stack->plane,
					g_card_plane_height[stack->card_orientation] / 2,
					card_offset_x + (stack->max_displayable > 0 
						? (stack->max_displayable - 1) * stack->padding : 0)
						+ card_width + 1,
					">%zu",
					overflow_right
				);
			}
			else // VERTICAL
			{
				ncplane_printf_yx(
					stack->plane,
					card_offset_y + (stack->max_displayable > 0 
						? (stack->max_displayable - 1) * stack->padding : 0)
						+ g_card_plane_height[stack->card_orientation],
					g_card_plane_width[stack->card_orientation] / 2,
					"v%zu",
					overflow_right
				);
			}
		}
	}
	
	stack->is_dirty = 0;
	return (RETURN_OK);
}
