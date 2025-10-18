
#include "deck.h"

int	deck_shuffle(struct s_deck *deck, unsigned int seed)
{
	int	i;
	int	j;
	t_card_desc	*temp;

	ft_srand(seed);
	i = deck->remaining - 1;
	j = 0;
	while (i > 0)
	{
		j = ft_rand(1, 10000) % (i + 1);
		// Swap deck->cards[i] and deck->cards[j]
		temp = deck->cards[i];
		deck->cards[i] = deck->cards[j];
		deck->cards[j] = temp;
		i--;
	}
	return (0);
}
