

#include "deck_display_anon.h"

int		deck_anon_init(
		struct s_deck_anon **deck,
		struct ncplane *parent,
		unsigned int x,
		unsigned int y,
		unsigned int card_count
)
{
	struct s_deck_anon *new_deck = {0};

	if (!deck || !parent)
		return (1);
	new_deck = ft_calloc(1, sizeof(struct s_deck_anon));
	if (!new_deck)
		return (1);
	new_deck->parent = parent;
	new_deck->is_dirty = 1;
	new_deck->x = x;
	new_deck->y = y;
	new_deck->card_count = card_count;
	struct ncplane_options popts = {
		.name = "deck_anon",
		.x = x,
		.y = y,
		.cols = CARD_WIDTH + 5,
		.rows = CARD_HEIGHT,
	};
	new_deck->plane = ncplane_create(parent, &popts);
	if (!new_deck->plane)
		return (free(new_deck), 1);
	(*deck) = new_deck;
	return (0);
}

void	deck_anon_destroy(
		struct s_deck_anon *deck
)
{
	if (!deck)
		return ;
	if (deck->visual)
		ncplane_destroy(deck->visual);
	if (deck->plane)
		ncplane_destroy(deck->plane);
	free(deck);
}

