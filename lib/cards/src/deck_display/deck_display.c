
#include "deck_display.h"

int			deck_display_create(
	struct s_deck_display **deck_display,
	struct ncplane *parent,
	struct s_deck *deck,
	unsigned int x,
	unsigned int y
)
{
	if (!deck_display || !parent || !deck)
		return (1);
	struct s_deck_display	*new_deck_display;
	new_deck_display = ft_calloc(1, sizeof(struct s_deck_display)); // TODO: move to free list
	if (!new_deck_display)
		return (1);
	new_deck_display->plane = ncplane_create(parent, &(struct ncplane_options){
		.y = x,
		.x = y,
		.rows = CARD_HEIGHT,
		.cols = CARD_WIDTH + 5,
		.flags = 0,
	});
	make_plane_transparent(new_deck_display->plane);
	if (!new_deck_display->plane)
		return (free(new_deck_display), 1);
	new_deck_display->deck = deck;
	new_deck_display->is_dirty = 1;
	new_deck_display->x = x;
	new_deck_display->y = y;
	(*deck_display) = new_deck_display;
	return (0);
}

void		deck_display_destroy(
	struct s_deck_display *deck_display
)
{
	if (!deck_display)
		return ;
	if (deck_display->plane)
		ncplane_destroy(deck_display->plane);
	free(deck_display); // TODO: move to free list
}
