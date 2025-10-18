
#include "deck.h"


struct s_deck	*deck_create(int include_jokers)
{
	struct s_deck	*deck;
	int				card_count = include_jokers ? 54 : 52;
	int				suit_idx;
	int				rank_idx;
	
	deck = ft_calloc(1, sizeof(struct s_deck));
	if (!deck)
		return (NULL);
	deck->has_jokers = include_jokers;
	deck->remaining = card_count;
	suit_idx = 0;
	while (suit_idx < SUIT_JOKER)
	{
		rank_idx = 0;
		while (rank_idx < RANK_JOKER_BLACK)
		{
			deck->cards[suit_idx * RANK_JOKER_BLACK + rank_idx] = ft_calloc(1, sizeof(t_card_desc));
			if (!deck->cards[suit_idx * RANK_JOKER_BLACK + rank_idx])
				return (deck_destroy(deck), NULL);
			deck->cards[suit_idx * RANK_JOKER_BLACK + rank_idx]->suit = (t_suit)suit_idx;
			deck->cards[suit_idx * RANK_JOKER_BLACK + rank_idx]->rank = (t_rank)rank_idx;
			rank_idx++;
		}
		suit_idx++;
	}
	if (include_jokers)
	{
		deck->cards[52] = ft_calloc(1, sizeof(t_card_desc));
		if (!deck->cards[52])
			return (deck_destroy(deck), NULL);
		deck->cards[52]->suit = SUIT_JOKER;
		deck->cards[52]->rank = RANK_JOKER_BLACK;
		deck->cards[53] = ft_calloc(1, sizeof(t_card_desc));
		if (!deck->cards[53])
			return (deck_destroy(deck), NULL);
		deck->cards[53]->suit = SUIT_JOKER;
		deck->cards[53]->rank = RANK_JOKER_RED;
	}
	return (deck);
}

void	deck_destroy(struct s_deck *deck)
{
	int	i;

	if (!deck)
		return;
	i = 0;
	while (i < deck->remaining)
	{
		if (deck->cards[i])
			free(deck->cards[i]);
		i++;
	}
	free(deck);
}
