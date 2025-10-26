#include "cards.h"
#include "game.h"

t_hashmap		cardmap;
unsigned int	card_dimensions[4] = {0, 0, 0, 0}; // width, height, h_width, h_height


static int	_load_jokers(void)
{
	char			*img_path = malloc(1024);
	t_card			*card;

	card = ft_calloc(1, sizeof(t_card));
	if (!card)
		return (1);
	card->rank = RANK_JOKER_BLACK;
	card->suit = SUIT_JOKER;
	sprintf(img_path, "%s%s %s.png", CARD_LOAD_PATH,
		suit_image_str[card->suit], rank_image_string[card->rank]
	);
	card->graphic = ncvisual_from_file(img_path);
	sprintf(img_path, "%s%s %s.png", CARD_LOAD_PATH,
		suit_image_str[card->suit], rank_image_string[card->rank]
	);
	card->graphic_h = ncvisual_from_file(img_path);
	ncvisual_rotate(card->graphic_h, 1.5707963267848966);
	if (hm_add_value(&cardmap, (t_card_desc *)card, card) != 0)
		return (1);
	card = ft_calloc(1, sizeof(t_card));
	if (!card)
		return (1);
	card->rank = RANK_JOKER_RED;
	card->suit = SUIT_JOKER;
	sprintf(img_path, "%s%s %s.png", CARD_LOAD_PATH,
		suit_image_str[card->suit], rank_image_string[card->rank]
	);
	card->graphic = ncvisual_from_file(img_path);
	sprintf(img_path, "%s%s %s.png", CARD_LOAD_PATH,
		suit_image_str[card->suit], rank_image_string[card->rank]
	);
	card->graphic_h = ncvisual_from_file(img_path);;
	ncvisual_rotate(card->graphic_h, 1.5707963267848966);
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
			sprintf(img_path, "%s%s %s.png",  CARD_LOAD_PATH,
				suit_image_str[card->suit], rank_image_string[card->rank]);
			card->graphic = ncvisual_from_file(img_path);
			sprintf(img_path, "%s%s %s.png",  CARD_LOAD_PATH,
				suit_image_str[card->suit], rank_image_string[card->rank]);
			card->graphic_h = ncvisual_from_file(img_path);
			if (!card->graphic)
			{
				dprintf(STDERR_FILENO, "Failed to load image: %s\n", img_path);
				free(img_path);
				free(card);
				return (1);
			}
			ncvisual_rotate(card->graphic_h, 1.5707963267848966);
			hm_add_value(&cardmap, (t_card_desc *)card, card);
			rank_idx++;
		}
		suit_idx++;
	}
	free(img_path);
	return (0);
}

void	*cardbacks[4] = {NULL, NULL, NULL, NULL};

static int _load_cardbacks(void)
{
	char	img_path[1024];
	sprintf(img_path, "%sBack Blue 2.png", CARD_LOAD_PATH);
	cardbacks[0] = ncvisual_from_file(img_path);
	if (!cardbacks[0])
		return (1);
	sprintf(img_path, "%sBack Blue 2.png", CARD_LOAD_PATH);
	cardbacks[0 + 2] = ncvisual_from_file(img_path);
	ncvisual_rotate(cardbacks[0 + 2], 1.5707963267848966);
	if (!cardbacks[0 + 2])
		return (1);
	sprintf(img_path, "%sBack Red 2.png", CARD_LOAD_PATH);
	cardbacks[1] = ncvisual_from_file(img_path);
	if (!cardbacks[1])
		return (1);
	sprintf(img_path, "%sBack Red 2.png", CARD_LOAD_PATH);
	cardbacks[1 + 2] = ncvisual_from_file(img_path);
	ncvisual_rotate(cardbacks[1 + 2], 1.5707963267848966);
	if (!cardbacks[1 + 2])
		return (1);
	return (0);
}

int	init_cards(void *nc)
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
	struct ncvisual_options vopts = {
		.n = notcurses_stdplane((struct notcurses *)nc),
		.x = 0,
		.y = 0,
		.blitter = NCBLIT_PIXEL,
		.scaling = NCSCALE_NONE,
		.flags = NCVISUAL_OPTION_CHILDPLANE,
	};
	struct ncplane *pl = ncvisual_blit(nc, cardbacks[0], &vopts);
	ncplane_dim_yx(pl, &card_dimensions[1], &card_dimensions[0]);
	ncplane_destroy(pl);
	pl = ncvisual_blit(nc, cardbacks[2], &vopts);
	ncplane_dim_yx(pl, &card_dimensions[3], &card_dimensions[2]);
	ncplane_destroy(pl);
	return (0);
}

void	destroy_cards(void)
{
	unsigned int	idx = 0;
	t_hashpair		*pair;

	while (idx < cardmap.max_hashes)
	{
		pair = cardmap.pairs[idx];
		while (pair)
		{
			if (pair && pair->value)
			{
				ncvisual_destroy(((t_card *)pair->value)->graphic);
				ncvisual_destroy(((t_card *)pair->value)->graphic_h);
				free((t_card *)pair->value);
				// free(pair);
			}
			pair->value = NULL;
			pair->key = NULL;
			pair = pair->next;
		}
		idx++;
	}
	hm_destroy(&cardmap);
	for (int i = 0; i < 4; i++)
	{
		if (cardbacks[i])
			ncvisual_destroy(cardbacks[i]);
	}
}
