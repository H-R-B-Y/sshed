
#include "game.h"
#include "cards.h"
#include <unistd.h>
#include <stdio.h>

int	init_cards(void *nc);

int	test_adding_cards(struct notcurses *nc, struct s_hand *hand, struct s_deck *deck)
{
	for (int i = 0; i < 5; i++)
	{
		t_card_desc	card;
		if (deck_draw_card(deck, &card))
			break ;
		if (hand_add_card(nc, hand, card) != 0)
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
	struct s_card_desc card_desc[4];
	
	for (int i = 0; i < 4; i++)
	{
		if (deck_draw_card(deck, (&card_desc[0])))
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
	hand_remove_card(nc, hand, card_desc[1]);
	deck_return_card(deck, card_desc[1]);
	hand_remove_card(nc, hand, card_desc[3]);
	deck_return_card(deck, card_desc[3]);
	hand_render(nc, hand);
	notcurses_get_blocking(nc, NULL);
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
		struct s_card_desc card;
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Filed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card(nc, pdisplay, card) != 0)
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
		t_card_desc card;
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card(nc, pdisplay2, card) != 0)
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
		t_card_desc	card;
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card(nc, pdisplay3, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n");
			return (1);
		}
	}
	pdisplay_render(nc, pdisplay3);
	notcurses_get_blocking(nc, NULL);

	/*
	Lets add some cards to the shed of top and left then change their displays
	*/
	for (int i = 0; i < 4; i++)
	{
		t_card_desc	card;
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card_shed(nc, pdisplay, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay shed\n");
			return (1);
		}
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card_shed(nc, pdisplay2, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay shed\n");
			return (1);
		}
	}
	pdisplay_render(nc, pdisplay);
	pdisplay_render(nc, pdisplay2);
	notcurses_get_blocking(nc, NULL);
	pdisplay_toggle_display(nc, pdisplay);
	pdisplay_toggle_display(nc, pdisplay2);
	notcurses_get_blocking(nc, NULL);
	pdisplay_clear_screen(nc, pdisplay);
	pdisplay_destroy(pdisplay);
	pdisplay_destroy(pdisplay2);
	pdisplay_destroy(pdisplay3);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	return (0);
}

int test_pile_display(struct notcurses *nc, struct s_hand *hand, struct s_deck *deck)
{
	(void)hand;
	struct s_pile_display	*pile_display;
	pile_display_create(&pile_display,
		notcurses_stdplane(nc), 2,
		CARD_ORIENTATION_VERTICAL,
		PILE_DISPLAY_VERTICAL,
		3, 3, 3
	);
	pile_display->is_face_down = 0;
	pile_display->is_visible = 1;
	pile_display_render(nc, pile_display);
	// notcurses_get_blocking(nc, NULL);
	for (int i = 0; i < 10; i++)
	{
		t_card_desc	card;

		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pile_display_add_card_top(nc, pile_display, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pile display\n");
			return (1);
		}
	}
	pile_display_render(nc, pile_display);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	notcurses_get_blocking(nc, NULL);
	pile_display_destroy(pile_display);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	return (0);
}

int	putting_it_all_together(struct notcurses *nc, struct s_hand *hand, struct s_deck *deck)
{
	struct s_pdisplay	*pdisplay;
	struct s_pile_display	*pile_display;
	unsigned int		width, height;

	ncplane_dim_yx(notcurses_stdplane(nc), &height, &width);
	pile_display_create(&pile_display,
		notcurses_stdplane(nc), CARD_WIDTH,
		CARD_ORIENTATION_VERTICAL,
		PILE_DISPLAY_HORIZONTAL,
		5,
		(width / 2) - (CARD_WIDTH / 2),
		(height / 2) - (CARD_HEIGHT / 2)
	);
	/*
	Deal 6 cards to the shed of the player and the opponent
	*/
	pdisplay_create(&pdisplay, notcurses_stdplane(nc), PDISPLAY_ORIENTATION_TOP);
	pdisplay_show_shed(nc, pdisplay);
	hand_show_shed(nc, hand);
	for (int i = 0; i < 6; i++)
	{
		t_card_desc	card;
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card_shed(nc, pdisplay, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay shed\n");
			return (1);
		}
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (hand_add_card_to_shed(nc, hand, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to hand shed\n");
			return (1);
		}
	}
	pile_display_render(nc, pile_display);
	pdisplay_render(nc, pdisplay);
	hand_render(nc, hand);
	notcurses_render(nc);
	sleep(1);
	// Then we give each player 5 cards in their hand
	for (int i = 0; i < 5; i++)
	{
		t_card_desc	card;
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card(nc, pdisplay, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay hand\n");
			return (1);
		}

		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (hand_add_card(nc, hand, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return (1);
		}
	}
	pdisplay_show_hand(nc, pdisplay);
	hand_show_hand(nc, hand);
	pile_display_render(nc, pile_display);
	pdisplay_render(nc, pdisplay);
	hand_render(nc, hand);
	notcurses_render(nc);
	sleep(1);
	/*
	Then a small event loop allowing the player to select a card using arrow keys
	*/
	while (1)
	{
		int key = notcurses_get_blocking(nc, NULL);
		if (key == 'q' || key == 'Q')
			break ;
		else if (key == NCKEY_RIGHT)
			hand_select_next_card(hand);
		else if (key == NCKEY_LEFT)
			hand_select_prev_card(hand);
		else if (key == NCKEY_SPACE)
		{
			hand_toggle_display(nc, hand);
			pdisplay_toggle_display(nc, pdisplay);
		}
		else if (key == NCKEY_ENTER || key == '\n' || key == '\r')
		{
			t_card_desc	selected_card;
			
			if (hand_pop_selected_card(hand, &selected_card))
			{
				dprintf(STDERR_FILENO, "Failed to pop card from hand\n");
			}
			pile_display_add_card_top(nc, pile_display, selected_card);
			int	ok, swap;

			if (pdisplay->card_count)
				{
				swap = pdisplay_show_hand(nc, pdisplay);
				ok = pdisplay_pop_card_index(pdisplay, 0, &selected_card);
				}
			else if (pdisplay->shed_count)
				{
				swap = pdisplay_show_shed(nc, pdisplay);
				ok = pdisplay_pop_card_index(pdisplay, pdisplay->shed_count % 6, &selected_card);
				}
			if (!swap)
					pdisplay_toggle_display(nc, pdisplay);
			if (ok == 0)
				pile_display_add_card_top(nc, pile_display, selected_card);
		}
		else
			dprintf(STDERR_FILENO, "Pressed key: %d\n", key);
		pile_display_render(nc, pile_display);
		pdisplay_render(nc, pdisplay);
		hand_render(nc, hand);
		notcurses_render(nc) ;
	}
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
	// if (test_adding_cards(nc, hand, deck) != 0)
	// {
	// 	dprintf(STDERR_FILENO, "Test adding cards failed\n");
	// 	// cleanup_client(nc, deck, hand);
	// 	notcurses_stop(nc);
	// 	return (1);
	// }
	// if (testing_removing_cards(nc, hand, deck) != 0)
	// {
	// 	dprintf(STDERR_FILENO, "Test removing cards failed\n");
	// 	// cleanup_client(nc, deck, hand);
	// 	notcurses_stop(nc);
	// 	return (1);
	// }
	// notcurses_get_blocking(nc, NULL);
	// if (test_pdisplay(nc, hand, deck) != 0)
	// {
	// 	dprintf(STDERR_FILENO, "Test pdisplay failed\n");
	// 	// cleanup_client(nc, deck, hand);
	// 	notcurses_stop(nc);
	// 	return (1);
	// }
	// deck_destroy(deck);
	// deck = deck_create(true);
	// if (!deck)
	// {
	// 	dprintf(STDERR_FILENO, "Failed to recreate deck\n");
	// 	// cleanup_client(nc, deck, hand);
	// 	notcurses_stop(nc);
	// 	return (1);
	// }
	// if (test_pile_display(nc, hand, deck) != 0)
	// {
	// 	dprintf(STDERR_FILENO, "Test pile display failed\n");
	// 	// cleanup_client(nc, deck, hand);
	// 	notcurses_stop(nc);
	// 	return (1);
	// }
	// cleanup_client(nc, deck, hand);
	if (putting_it_all_together(nc, hand, deck) != 0)
	{
		dprintf(STDERR_FILENO, "Putting it all together test failed\n");
		// cleanup_client(nc, deck, hand);
		notcurses_stop(nc);
		return (1);
	}
	notcurses_stop(nc);
	return (0);
}
