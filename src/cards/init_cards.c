#include "cards.h"
#include "game.h"

t_hashmap	cardmap;

static int	_load_jokers(void)
{
	char	*img_path = malloc(1024);
	t_card	*card;

	card = ft_calloc(1, sizeof(t_card));
	if (!card)
		return (1);
	card->rank = RANK_JOKER_BLACK;
	card->suit = SUIT_JOKER;
	sprintf(img_path, "images/Medium/%s %s.png",
		suit_image_str[card->suit], rank_image_string[card->rank]
	);
	card->graphic = ncvisual_from_file(img_path);
	if (hm_add_value(&cardmap, (t_card_desc *)card, card) != 0)
		return (1);
	card = ft_calloc(1, sizeof(t_card));
	if (!card)
		return (1);
	card->rank = RANK_JOKER_RED;
	card->suit = SUIT_JOKER;
	sprintf(img_path, "images/Medium/%s %s.png",
		suit_image_str[card->suit], rank_image_string[card->rank]
	);
	card->graphic = ncvisual_from_file(img_path);
	free(img_path);
	if (hm_add_value(&cardmap, (t_card_desc *)card, card) != 0)
		return (1);
	return (0);
}

static int	_load_cards(void)
{
	int		suit_idx;
	int		rank_idx;
	char	*img_path = malloc(1024);
	t_card	*card;

	suit_idx = 0;
	while (suit_idx < SUIT_JOKER)
	{
		rank_idx = 0;
		while (rank_idx < RANK_JOKER_BLACK)
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

void	*cardbacks[2] = {NULL, NULL};

static int _load_cardbacks(void)
{
	cardbacks[0] = ncvisual_from_file("images/Medium/Back Blue 2.png");
	if (!cardbacks[0])
		return (1);
	cardbacks[1] = ncvisual_from_file("images/Medium/Back Red 2.png");
	if (!cardbacks[1])
		return (1);
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
	_load_cards();
	_load_jokers();
	_load_cardbacks();
	return (0);
}

