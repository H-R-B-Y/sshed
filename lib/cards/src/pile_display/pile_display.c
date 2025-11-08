
#include "pile_display.h"

int		pile_display_create(
	struct s_pile_display **pile_display,
	struct ncplane *parent,
	unsigned int padding,
	enum e_card_orientation card_orientation,
	enum e_pile_display_orientation orientation,
	unsigned int max_stack,
	unsigned int x,
	unsigned int y
)
{
	if (!pile_display || !parent)
		return (1);
	struct s_pile_display	*new_pile_display;
	new_pile_display = ft_calloc(1, sizeof(struct s_pile_display)); // TODO: move to free list
	if (!new_pile_display)
		return (1);
	unsigned int card_width = (card_orientation == CARD_ORIENTATION_HORIZONTAL)
		? CARD_H_WIDTH
		: CARD_WIDTH;
	unsigned int card_height = (card_orientation == CARD_ORIENTATION_HORIZONTAL)
		? CARD_H_HEIGHT
		: CARD_HEIGHT;
	
	new_pile_display->orientation = orientation;
	/* Calculate plane dimensions depending on pile orientation.
	   If the pile is horizontal, span horizontally (cols) by stacking cards
	   If vertical, span vertically (rows) by stacking cards. */
	unsigned int plane_cols = card_width + 5;
	unsigned int plane_rows = card_height + 2;
	if (max_stack > 0)
	{
		if (orientation == PILE_DISPLAY_HORIZONTAL)
			plane_cols += (max_stack - 1) * padding;
		else
			plane_rows += (max_stack - 1) * padding;
	}

	new_pile_display->plane = ncplane_create(parent, &(struct ncplane_options){
		.x = x,
		.y = y,
		.cols = plane_cols,
		.rows = plane_rows,
		.flags = 0,
	});
	make_plane_transparent(new_pile_display->plane);
	if (!new_pile_display->plane)
		return (free(new_pile_display), 1);
	new_pile_display->cards = cdll_create();
	if (!new_pile_display->cards)
		return (ncplane_destroy(new_pile_display->plane), free(new_pile_display), 1);
	new_pile_display->padding = padding;
	new_pile_display->card_orientation = card_orientation;
	new_pile_display->is_face_down = 0;
	new_pile_display->max_stack = max_stack;
	new_pile_display->is_visible = 1;
	new_pile_display->is_dirty = 0;
	new_pile_display->x = x;
	new_pile_display->y = y;
	new_pile_display->selected = -1;
	(*pile_display) = new_pile_display;
	return (0);
}

void	pile_display_destroy(
	struct s_pile_display *pile_display
)
{
	size_t				count;
	struct s_cdll_node	*current, *next;
	struct s_card_plane	*card_plane;

	if (!pile_display)
		return ;
	if (pile_display->plane)
		ncplane_destroy(pile_display->plane);
	// Free cards cdlls
	if (pile_display->cards)
	{
		current = pile_display->cards->head;
		count = pile_display->cards->count;
		for (size_t idx = 0; idx < count; idx++)
		{
			card_plane = (struct s_card_plane *)current->data;
			next = current->next;
			card_plane_destroy(card_plane);
			free(current); // TODO: make this use a free list
			current = next;
		}
		free(pile_display->cards);
	}
	free(pile_display); // TODO: move to free list
}
