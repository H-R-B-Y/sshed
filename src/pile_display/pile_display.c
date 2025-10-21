
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
	new_pile_display->plane = ncplane_create(parent, &(struct ncplane_options){
		.x = x,
		.y = y,
		// So the width is card width (or H width) + (max_stack * padding)
		.cols = card_width + (max_stack > 0 ? (max_stack - 1) * padding : 0) + 5,
		// So the height is card height (or H height) + (max_stack * padding)
		.rows = card_height + (max_stack > 0 ? (max_stack - 1) * padding : 0),
		.flags = 0,
	});
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
	(*pile_display) = new_pile_display;
	return (0);
}

void	pile_display_destroy(
	struct s_pile_display *pile_display
)
{
	if (!pile_display)
		return ;
	if (pile_display->plane)
		ncplane_destroy(pile_display->plane);
	// Free cards cdll
	if (pile_display->cards)
	{
		dprintf(STDERR_FILENO, "Freeing pile_display cards cdll with %zu"
			"cards will permanently remove card descriptors from memory!\n",
			pile_display->cards->count);
		struct s_cdll_node	*current = pile_display->cards->head;
		struct s_cdll_node	*next;
		size_t		count = pile_display->cards->count;
		for (size_t idx = 0; idx < count; idx++)
		{
			struct s_card_plane *card_plane = (struct s_card_plane *)current->data;
			next = current->next;
			card_plane_destroy(card_plane);
			free(current); // TODO: make this use a free list
			current = next;
		}
		free(pile_display->cards);
	}
	free(pile_display); // TODO: move to free list
}
