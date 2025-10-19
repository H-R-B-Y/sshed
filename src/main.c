
#include "game.h"
#include "cards.h"
#include <unistd.h>
#include <stdio.h>

int	init_cards(void);



int main(void)
{
	struct notcurses_options opts = {
		.flags = NCOPTION_SUPPRESS_BANNERS,
		.loglevel = NCLOGLEVEL_FATAL,
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
		if (hand_add_card(nc, hand, card_desc) != 0)
		{
			deck_destroy(deck);
			hand_destroy(hand);
			notcurses_stop(nc);
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return 1;
		}
	}
	hand->card_selected[0] = 2; // select the third card
	hand_render(nc, hand);
	// notcurses_get_blocking(nc, NULL);
	notcurses_get_blocking(nc, NULL);
	for (int i = 0; i < 5; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (hand_add_card(nc, hand, card_desc) != 0)
		{
			deck_destroy(deck);
			hand_destroy(hand);
			notcurses_stop(nc);
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return 1;
		}
	}
	hand->card_selected[0] = 4; // select the fifth card
	// hand_render(nc, hand);
	notcurses_get_blocking(nc, NULL);
	hand_remove_card(nc, hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	// ncplane_erase(plane);
	hand_remove_card(nc, hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	hand_remove_card(nc, hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	hand_remove_card(nc, hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	hand_remove_card(nc, hand, ((struct s_card_plane *)hand->cards->content)->card_desc);
	// hand_render(nc, hand);
	notcurses_get_blocking(nc, NULL);
	/*
	Now to test the shed
	*/
	for (int i = 0; i < 6; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (hand_add_card_to_shed(nc, hand, card_desc) != 0)
		{
			deck_destroy(deck);
			hand_destroy(hand);
			notcurses_stop(nc);
			dprintf(STDERR_FILENO, "Failed to add card to shed\n");
			return 1;
		}
	}
	debug_print_shed(hand);
	hand->card_selected[1] = 2; // shed can only select 0 - 2
	hand_toggle_display(nc, hand);
	debug_print_shed(hand);
	notcurses_get_blocking(nc, NULL);
	hand_toggle_display(nc, hand);
	notcurses_get_blocking(nc, NULL);
	hand_remove_card_shed(nc, hand, hand->shed[0]->card_desc);
	hand_remove_card_shed(nc, hand, hand->shed[1]->card_desc);
	hand_remove_card_shed(nc, hand, hand->shed[2]->card_desc);
	notcurses_get_blocking(nc, NULL);
	hand_toggle_display(nc, hand);
	notcurses_get_blocking(nc, NULL);
	hand_destroy(hand);
	deck_destroy(deck);
	// should really unload the cards here but whatever
	notcurses_stop(nc);
	return 0;
}