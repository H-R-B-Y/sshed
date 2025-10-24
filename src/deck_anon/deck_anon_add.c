
#include "deck_display_anon.h"

int		deck_anon_inc(
		struct s_deck_anon *deck
)
{
	if (!deck)
		return (1);
	deck->card_count++;
	deck->is_dirty = 1;
	return (0);
}

int		deck_anon_dec(
		struct s_deck_anon *deck
)
{
	if (!deck)
		return (1);
	if (deck->card_count == 0)
		return (0);
	deck->card_count--;
	deck->is_dirty = 1;
	return (0);
}

int		deck_anon_add(
		struct s_deck_anon *deck,
		unsigned int add
)
{
	if (!deck)
		return (1);
	deck->card_count += add;
	deck->is_dirty = 1;
	return (0);
}

int		deck_anon_sub(
		struct s_deck_anon *deck,
		unsigned int sub
)
{
	if (!deck)
		return (1);
	if (sub > deck->card_count)
		sub = deck->card_count;
	deck->card_count -= sub;
	deck->is_dirty = 1;
	return (0);
}