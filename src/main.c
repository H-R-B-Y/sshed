
#include "game.h"
#include "cards.h"
#include <unistd.h>
#include <stdio.h>

int	init_cards(void);

// int	render_cards_default(struct notcurses *nc, struct ncplane *plane)
// {
// 	unsigned int	suit_idx;
// 	unsigned int	rank_idx;
// 	t_card			*card;
// 	ncplane_erase(plane);
// 	suit_idx = 0;
// 	while (suit_idx < SUIT_JOKER)
// 	{
// 		rank_idx = 0;
// 		while (rank_idx < RANK_JOKER_BLACK)
// 		{
// 			card = hm_get_value(&cardmap, &(t_card_desc){
// 				.suit = (t_suit)suit_idx,
// 				.rank = (t_rank)rank_idx
// 			});
// 			if (card && card->graphic)
// 			{
// 				struct ncvisual_options vopts = {
// 					.n = plane,
// 					.x = rank_idx * 9,
// 					.y = suit_idx * 6,
// 					.blitter = NCBLIT_PIXEL,
// 					.flags = NCVISUAL_OPTION_CHILDPLANE,
// 				};
// 				// dprintf(STDERR_FILENO, "Rendering card: %s of %s at (%u, %u)\n",
// 				// 	rank_str[card->rank], suit_str[card->suit],
// 				// 	vopts.x, vopts.y);
// 				ncvisual_blit(nc, card->graphic, &vopts);
// 			}
// 			else
// 			{
// 				dprintf(STDERR_FILENO, "Card not found: %s of %s\n",
// 					rank_str[(t_rank)rank_idx], suit_str[(t_suit)suit_idx]);
// 			}
// 			rank_idx++;
// 		}
// 		suit_idx++;
// 	}
// 	rank_idx = 0;
// 	card = hm_get_value(&cardmap, &(t_card_desc){.suit = SUIT_JOKER, .rank = RANK_JOKER_BLACK});
// 	if (card && card->graphic)
// 	{
// 		struct ncvisual_options vopts = {
// 			.n = plane,
// 			.x = rank_idx * 9,
// 			.y = suit_idx * 6,
// 			.blitter = NCBLIT_PIXEL,
// 			.flags= NCVISUAL_OPTION_CHILDPLANE,
// 		};
// 		ncvisual_blit(nc, card->graphic, &vopts);
// 	}
// 	rank_idx = 1;
// 	card = hm_get_value(&cardmap, &(t_card_desc){.suit = SUIT_JOKER, .rank = RANK_JOKER_RED});
// 	if (card && card->graphic)
// 	{
// 		struct ncvisual_options vopts = {
// 			.n = plane,
// 			.x = rank_idx * 9,
// 			.y = suit_idx * 6,
// 			.blitter = NCBLIT_PIXEL,
// 			.flags= NCVISUAL_OPTION_CHILDPLANE,
// 		};
// 		ncvisual_blit(nc, card->graphic, &vopts);
// 	}
// 	if (notcurses_render(nc) != 0)
// 		return (1);
// 	return (0);
// }

// int	render_deck(struct notcurses *nc, struct ncplane *plane)
// {
// 	struct s_deck	*deck;
// 	t_card_desc	*card_desc;
// 	int			idx;

// 	deck = deck_create(0);
// 	if (!deck)
// 		return (1);
// 	deck_shuffle(deck, (unsigned int)time(NULL));
// 	idx = 0;
// 	while (deck->remaining > 0)
// 	{
// 		card_desc = deck_draw_card(deck);
// 		if (!card_desc)
// 			break ;
// 		t_card	*card = hm_get_value(&cardmap, card_desc);
// 		if (card && card->graphic)
// 		{
// 			struct ncvisual_options vopts = {
// 				.n = plane,
// 				.x = (idx % 13) * 10,
// 				.y = (idx / 13) * 6,
// 				.blitter = NCBLIT_PIXEL,
// 				.flags = NCVISUAL_OPTION_CHILDPLANE,
// 			};
// 			ncvisual_blit(nc, card->graphic, &vopts);
// 			if (notcurses_render(nc) != 0)
// 			{
// 				deck_destroy(deck);
// 				return (1);
// 			}
// 		}
// 		idx++;
// 	}
// 	deck_destroy(deck);
// 	return (0);
// }

int main(void)
{
	struct notcurses_options opts = {
		.flags = NCOPTION_SUPPRESS_BANNERS,
		.loglevel = NCLOGLEVEL_WARNING,
	};

	printf("Initializing notcurses...\n");
	fflush(stdout);
	
	struct notcurses *nc = notcurses_init(&opts, NULL);
	if (!nc)
	{
		printf("Failed to init notcurses\n");
		return 1;
	}
	
	printf("Notcurses initialized successfully!\n");
	// this needs to take up the whole screen so we need to get the size of the terminal

	unsigned int rows, cols;
	notcurses_term_dim_yx(nc, &rows, &cols);

	struct ncplane_options nopts = {
		.x = 0,
		.y = 0,
		.rows = rows,
		.cols = cols,
		// .flags = NCPLANE_OPTION_HORALIGNED | NCPLANE_OPTION_VERALIGNED,
	};
	struct ncplane *plane = ncplane_create(notcurses_stdplane(nc), &nopts);
	if (!plane)
	{
		notcurses_stop(nc);
		dprintf(STDERR_FILENO, "Failed to create ncplane\n");
		return 1;
	}

	// need to draw the plane
	if (notcurses_render(nc) != 0)
	{
		notcurses_stop(nc);
		dprintf(STDERR_FILENO, "Failed to render ncplane\n");
		return 1;
	}

	if (init_cards() != 0)
	{
		notcurses_stop(nc);
		dprintf(STDERR_FILENO, "Failed to init cards\n");
		return 1;
	}

	// render all the cards
	// if (render_cards_default(nc, plane) != 0)
	// {
	// 	notcurses_stop(nc);
	// 	dprintf(STDERR_FILENO, "Failed to render cards\n");
	// 	return 1;
	// }
	// render_deck(nc, plane);
	struct s_hand	*hand;
	if (hand_create(&hand, plane, cols, rows - 7, 0) != 0)
	{
		notcurses_stop(nc);
		dprintf(STDERR_FILENO, "Failed to create hand\n");
		return 1;
	}
	struct s_deck	*deck = deck_create(1);
	if (!deck)
	{
		hand_destroy(hand);
		notcurses_stop(nc);
		dprintf(STDERR_FILENO, "Failed to create deck\n");
		return 1;
	}
	deck_shuffle(deck, (unsigned int)time(NULL));
	for (int i = 0; i < 5; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (hand_add_card(hand, card_desc) != 0)
		{
			deck_destroy(deck);
			hand_destroy(hand);
			notcurses_stop(nc);
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return 1;
		}
	}
	hand->card_selected = 2; // select the third card
	hand_render(nc, hand);
	notcurses_render(nc);
	// notcurses_get_blocking(nc, NULL);
	notcurses_get_blocking(nc, NULL);
	for (int i = 0; i < 5; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (hand_add_card(hand, card_desc) != 0)
		{
			deck_destroy(deck);
			hand_destroy(hand);
			notcurses_stop(nc);
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return 1;
		}
	}
	hand->card_selected = 4; // select the fifth card
	hand_render(nc, hand);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	hand_remove_card(hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	ncplane_erase(plane);
	hand_render(nc, hand);
	hand_remove_card(hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	hand_render(nc, hand);
	hand_remove_card(hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	hand_render(nc, hand);
	hand_remove_card(hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	hand_render(nc, hand);
	hand_remove_card(hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	hand_render(nc, hand);
	// notcurses_refresh(nc, NULL, NULL);
	hand_render(nc, hand);
	notcurses_get_blocking(nc, NULL);
	notcurses_get_blocking(nc, NULL);
	// should really unload the cards here but whatever
	notcurses_stop(nc);
	return 0;
}