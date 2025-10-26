
#include "deck.h"


struct s_deck	*deck_create(int include_jokers)
{
	struct s_deck	*deck;
	int				card_count = include_jokers ? 54 : 52;
	int				card_idx;
	int				suit_idx;
	int				rank_idx;
	
	deck = ft_calloc(1, sizeof(struct s_deck));
	if (!deck)
		return (NULL);
	deck->has_jokers = include_jokers;
	deck->remaining = card_count;
	suit_idx = 0;
	deck->arena = ft_calloc(
		card_count, sizeof(t_card_desc)
	);
	if (!deck->arena)
		return (deck_destroy(deck), NULL);
	card_idx = 0;
	while (suit_idx < SUIT_JOKER)
	{
		rank_idx = 0;
		while (rank_idx < RANK_JOKER_BLACK)
		{
			((t_card_desc *)deck->arena)[card_idx] = (t_card_desc){
				.rank = (t_rank)rank_idx,
				.suit = (t_suit)suit_idx
			};
			deck->cards[suit_idx * RANK_JOKER_BLACK + rank_idx] = ((t_card_desc *)deck->arena) + card_idx;
			rank_idx++;
			card_idx++;
		}
		suit_idx++;
	}
	if (include_jokers)
	{
		((t_card_desc *)deck->arena)[card_idx] = (t_card_desc){
			.rank = RANK_JOKER_BLACK,
			.suit = SUIT_JOKER
		};
		deck->cards[52] = ((t_card_desc *)deck->arena) + card_idx;
		card_idx++;
		((t_card_desc *)deck->arena)[card_idx] = (t_card_desc){
			.rank = RANK_JOKER_RED,
			.suit = SUIT_JOKER
		};
		deck->cards[53] = ((t_card_desc *)deck->arena) + card_idx;
	}
	return (deck);
}

void	deck_destroy(struct s_deck *deck)
{
	if (!deck)
		return;
	if (deck->arena)
		free(deck->arena);
	ft_memset(deck->cards, 0, sizeof(deck->cards));
	free(deck);
}
