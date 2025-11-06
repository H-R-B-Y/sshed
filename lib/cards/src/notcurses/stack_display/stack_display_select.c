/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_display_select.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 11:43:00 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/06 11:35:54 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "notcurses/stack_display.h"

t_returncode	stack_display_set_selected_card(
	struct s_stack_display *stack,
	ssize_t selected_index
)
{
	if (!stack)
		return (RETURN_ERROR);
	
	// Validate the index
	if (selected_index >= (ssize_t)stack->card_plane_count && selected_index != -1)
		return (RETURN_ERROR);
	
	stack->selected_index = selected_index;
	
	// Update the selected card pointer
	if (selected_index >= 0 && selected_index < (ssize_t)stack->card_plane_count 
		&& stack->card_planes[selected_index])
	{
		stack->selected_card = &stack->card_planes[selected_index]->card;
	}
	else
	{
		stack->selected_card = NULL;
	}
	
	stack->is_dirty = 1;
	return (RETURN_OK);
}

t_returncode	stack_display_select_next_card(
	struct s_stack_display *stack
)
{
	ssize_t	next_index;

	if (!stack)
		return (RETURN_ERROR);
	
	// If no card is selected or we're at the end, wrap to start
	if (stack->selected_index < 0 
		|| stack->selected_index >= (ssize_t)stack->card_plane_count - 1)
	{
		next_index = 0;
	}
	else
	{
		next_index = stack->selected_index + 1;
	}
	
	// If there are no cards, select none
	if (stack->card_plane_count == 0)
		next_index = -1;
	
	return (stack_display_set_selected_card(stack, next_index));
}

t_returncode	stack_display_select_previous_card(
	struct s_stack_display *stack
)
{
	ssize_t	prev_index;

	if (!stack)
		return (RETURN_ERROR);
	
	// If no card is selected or we're at the start, wrap to end
	if (stack->selected_index <= 0)
	{
		if (stack->card_plane_count > 0)
			prev_index = (ssize_t)stack->card_plane_count - 1;
		else
			prev_index = -1;
	}
	else
	{
		prev_index = stack->selected_index - 1;
	}
	
	return (stack_display_set_selected_card(stack, prev_index));
}
