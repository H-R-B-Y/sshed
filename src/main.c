
#include "game.h"
#include "cards.h"
#include <unistd.h>
#include <stdio.h>

int	init_cards(void);

int	render_cards_default(struct notcurses *nc, struct ncplane *plane)
{
	unsigned int	suit_idx;
	unsigned int	rank_idx;
	t_card			*card;
	ncplane_erase(plane);
	suit_idx = 0;
	while (suit_idx < SUIT_COUNT)
	{
		rank_idx = 0;
		while (rank_idx < RANK_COUNT)
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
	if (notcurses_render(nc) != 0)
		return (1);
	return (0);
}

int main(void)
{
	struct notcurses_options opts = {
		.flags = NCOPTION_SUPPRESS_BANNERS,
	};

	struct notcurses *nc = notcurses_init(&opts, NULL);
	if (!nc)
	{
		printf("Failed to init notcurses\n");
		return 1;
	}
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
	if (render_cards_default(nc, plane) != 0)
	{
		notcurses_stop(nc);
		dprintf(STDERR_FILENO, "Failed to render cards\n");
		return 1;
	}

	notcurses_get_blocking(nc, NULL);
	notcurses_stop(nc);
	// should really unload the cards here but whatever
	return 0;
}