#include "cards.h"
#include "game.h"

t_hashmap	cardmap;

unsigned int hash_card(t_card_desc *card_desc)
{
	return ((unsigned int)card_desc->suit * RANK_COUNT + (unsigned int)card_desc->rank);
}

int		compare_card(t_card_desc *a, t_card_desc *b)
{
	if (a->rank == b->rank && a->suit == b->suit)
		return (0);
	return (1);
}

int	load_cards(void)
{
	int		suit_idx;
	int		rank_idx;
	char	*img_path = malloc(1024);
	t_card	*card;

	suit_idx = 0;
	while (suit_idx < SUIT_COUNT)
	{
		rank_idx = 0;
		while (rank_idx < RANK_COUNT)
		{
			card = ft_calloc(1, sizeof(t_card));
			if (!card)
				return (1);
			card->suit = (t_suit)suit_idx;
			card->rank = (t_rank)rank_idx;
			sprintf(img_path, "images/Medium/%s %s.png", 
				suit_image_str[card->suit], rank_image_string[card->rank]);
			// dprintf(STDERR_FILENO, "Loading card image: %s\n", img_path);
			card->graphic = ncvisual_from_file(img_path);
			if (!card->graphic)
			{
				dprintf(STDERR_FILENO, "Failed to load image: %s\n", img_path);
				free(img_path);
				free(card);
				return (1);
			}
			hm_add_value(&cardmap, (t_card_desc *)card, card);
			rank_idx++;
		}
		suit_idx++;
	}
	free(img_path);
	return (0);
}

int	init_cards(void)
{
	cardmap.hash_cmp = (t_hashcmp)compare_card;
	cardmap.hash_key = (t_hashfnc)hash_card;
	cardmap.max_hashes = (SUIT_COUNT * RANK_COUNT) + 1;
	cardmap.pairs = ft_calloc(cardmap.max_hashes + 1, sizeof(t_hashpair *));
	if (!cardmap.pairs)
		return (1);
	load_cards();
	return (0);
}

