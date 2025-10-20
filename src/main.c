
#include "game.h"
#include "cards.h"
#include <unistd.h>
#include <stdio.h>

int	init_cards(void);

int	test_adding_cards(struct notcurses *nc, struct s_hand *hand, struct s_deck *deck)
{
	for (int i = 0; i < 5; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (hand_add_card(nc, hand, card_desc) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return (1);
		}
	}
	hand->card_selected[0] = 2; // select the third card
	hand_render(nc, hand);
	notcurses_get_blocking(nc, NULL);
	hand_return_to_deck(nc, hand, deck);
	// validate that the hand is empty and deck has 52 + jokers?
	if (hand->card_count != 0)
	{
		dprintf(STDERR_FILENO, "Hand is not empty after returning cards to deck\n");
		return (1);
	}
	if (deck->remaining != 54)
	{
		dprintf(STDERR_FILENO, "Deck does not have all cards after returning from hand\n");
		return (1);
	}
	return (0);
}

int	testing_removing_cards(struct notcurses *nc, struct s_hand *hand, struct s_deck *deck)
{
	struct s_card_desc *card_desc[4];
	
	for (int i = 0; i < 4; i++)
	{
		card_desc[i] = deck_draw_card(deck);
		if (!card_desc[i])
		{
			dprintf(STDERR_FILENO, "Failed to draw card from deck\n");
			return (1);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (hand_add_card(nc, hand, card_desc[i]) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return (1);
		}
	}
	hand_render(nc, hand);
	notcurses_get_blocking(nc, NULL);
	// Now remove two cards
	hand_remove_card(nc, hand, card_desc[1]);
	deck_return_card(deck, card_desc[1]);
	hand_remove_card(nc, hand, card_desc[3]);
	deck_return_card(deck, card_desc[3]);
	hand_render(nc, hand);
	notcurses_get_blocking(nc, NULL);
	// Validate hand has 2 cards left
	if (hand->card_count != 2)
	{
		dprintf(STDERR_FILENO, "Hand does not have 2 cards after removing 2 cards\n");
		return (1);
	}
	// Validate deck has 54 - 2 = 52 cards
	if (deck->remaining != 52)
	{
		dprintf(STDERR_FILENO, "Deck does not have 52 cards after removing 2 cards from hand\n");
		return (1);
	}
	hand_return_to_deck(nc, hand, deck);
	return (0);
}

int	test_pdisplay(struct notcurses *nc, struct s_hand *hand, struct s_deck *deck)
{
	(void)hand;
	struct s_pdisplay	*pdisplay;

	pdisplay_create(&pdisplay, notcurses_stdplane(nc), PDISPLAY_ORIENTATION_TOP);
	for (int i = 0; i < 5; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (pdisplay_add_card(nc, pdisplay, card_desc) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n");
			return (1);
		}
	}
	pdisplay_render(nc, pdisplay);
	notcurses_get_blocking(nc, NULL);
	struct s_pdisplay	*pdisplay2;
	pdisplay_create(&pdisplay2, notcurses_stdplane(nc), PDISPLAY_ORIENTATION_LEFT);
	for (int i = 0; i < 3; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (pdisplay_add_card(nc, pdisplay2, card_desc) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n");
			return (1);
		}
	}
	pdisplay_render(nc, pdisplay2);
	notcurses_get_blocking(nc, NULL);
	struct s_pdisplay	*pdisplay3;
	pdisplay_create(&pdisplay3, notcurses_stdplane(nc), PDISPLAY_ORIENTATION_RIGHT);
	for (int i = 0; i < 3; i++)
	{
		t_card_desc	*card_desc = deck_draw_card(deck);
		if (!card_desc)
			break ;
		if (pdisplay_add_card(nc, pdisplay3, card_desc) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n");
			return (1);
		}
	}
	pdisplay_render(nc, pdisplay3);
	notcurses_get_blocking(nc, NULL);
	pdisplay_clear_screen(nc, pdisplay);
	pdisplay_destroy(pdisplay);
	pdisplay_destroy(pdisplay2);
	pdisplay_destroy(pdisplay3);
	return (0);
}


int	main(void) // should take some args later
{
	// Need to also initialise the client connection
	struct notcurses	*nc;
	struct s_deck		*deck;
	struct s_hand		*hand;

	if (init_client(&nc, &deck, &hand) != 0)
	{
		dprintf(STDERR_FILENO, "Failed to initialize client\n");
		return (1);
	}
	sleep(1); // pause to see the initial state
	if (test_adding_cards(nc, hand, deck) != 0)
	{
		dprintf(STDERR_FILENO, "Test adding cards failed\n");
		// cleanup_client(nc, deck, hand);
		notcurses_stop(nc);
		return (1);
	}
	if (testing_removing_cards(nc, hand, deck) != 0)
	{
		dprintf(STDERR_FILENO, "Test removing cards failed\n");
		// cleanup_client(nc, deck, hand);
		notcurses_stop(nc);
		return (1);
	}
	// notcurses_get_blocking(nc, NULL);
	if (test_pdisplay(nc, hand, deck) != 0)
	{
		dprintf(STDERR_FILENO, "Test pdisplay failed\n");
		// cleanup_client(nc, deck, hand);
		notcurses_stop(nc);
		return (1);
	}
	// cleanup_client(nc, deck, hand);
	notcurses_stop(nc);
	return (0);
}
