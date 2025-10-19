
#include "game.h"

int	render_cards_default(struct notcurses *nc, struct ncplane *plane)
{
	unsigned int	suit_idx;
	unsigned int	rank_idx;
	t_card			*card;
	ncplane_erase(plane);
	suit_idx = 0;
	while (suit_idx < SUIT_JOKER)
	{
		rank_idx = 0;
		while (rank_idx < RANK_JOKER_BLACK)
		{
			card = hm_get_value(&cardmap, &(t_card_desc){
				.suit = (t_suit)suit_idx,
				.rank = (t_rank)rank_idx
			});
			if (card && card->graphic)
			{
				struct ncvisual_options vopts = {
					.n = plane,
					.x = rank_idx * 9,
					.y = suit_idx * 6,
					.blitter = NCBLIT_PIXEL,
					.flags = NCVISUAL_OPTION_CHILDPLANE,
				};
				// dprintf(STDERR_FILENO, "Rendering card: %s of %s at (%u, %u)\n",
				// 	rank_str[card->rank], suit_str[card->suit],
				// 	vopts.x, vopts.y);
				ncvisual_blit(nc, card->graphic, &vopts);
			}
			else
			{
				dprintf(STDERR_FILENO, "Card not found: %s of %s\n",
					rank_str[(t_rank)rank_idx], suit_str[(t_suit)suit_idx]);
			}
			rank_idx++;
		}
		suit_idx++;
	}
	rank_idx = 0;
	card = hm_get_value(&cardmap, &(t_card_desc){.suit = SUIT_JOKER, .rank = RANK_JOKER_BLACK});
	if (card && card->graphic)
	{
		struct ncvisual_options vopts = {
			.n = plane,
			.x = rank_idx * 9,
			.y = suit_idx * 6,
			.blitter = NCBLIT_PIXEL,
			.flags= NCVISUAL_OPTION_CHILDPLANE,
		};
		ncvisual_blit(nc, card->graphic, &vopts);
	}
	rank_idx = 1;
	card = hm_get_value(&cardmap, &(t_card_desc){.suit = SUIT_JOKER, .rank = RANK_JOKER_RED});
	if (card && card->graphic)
	{
		struct ncvisual_options vopts = {
			.n = plane,
			.x = rank_idx * 9,
			.y = suit_idx * 6,
			.blitter = NCBLIT_PIXEL,
			.flags= NCVISUAL_OPTION_CHILDPLANE,
		};
		ncvisual_blit(nc, card->graphic, &vopts);
	}
	if (notcurses_render(nc) != 0)
		return (1);
	return (0);
}

int	render_deck(struct notcurses *nc, struct ncplane *plane)
{
	struct s_deck	*deck;
	t_card_desc	*card_desc;
	int			idx;

	deck = deck_create(0);
	if (!deck)
		return (1);
	deck_shuffle(deck, (unsigned int)time(NULL));
	idx = 0;
	while (deck->remaining > 0)
	{
		card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		t_card	*card = hm_get_value(&cardmap, card_desc);
		if (card && card->graphic)
		{
			struct ncvisual_options vopts = {
				.n = plane,
				.x = (idx % 13) * 10,
				.y = (idx / 13) * 6,
				.blitter = NCBLIT_PIXEL,
				.flags = NCVISUAL_OPTION_CHILDPLANE,
			};
			ncvisual_blit(nc, card->graphic, &vopts);
			if (notcurses_render(nc) != 0)
			{
				deck_destroy(deck);
				return (1);
			}
		}
		idx++;
	}
	deck_destroy(deck);
	return (0);
}