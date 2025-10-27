
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
		if (hand_add_card(hand, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return (1);
		}
	}
	hand->card_selected[0] = 2; // select the third card
	hand_render(hand);
	notcurses_get_blocking(nc, NULL);
	hand_return_to_deck(hand, deck);
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
		if (hand_add_card(hand, card_desc[i]) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to hand\n");
			return (1);
		}
	}
	hand_render(hand);
	notcurses_get_blocking(nc, NULL);
	hand_remove_card(hand, card_desc[1]);
	deck_return_card(deck, card_desc[1]);
	hand_remove_card(hand, card_desc[3]);
	deck_return_card(deck, card_desc[3]);
	hand_render(hand);
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
	hand_return_to_deck(hand, deck);
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
		if (pdisplay_add_card(pdisplay, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n");
			return (1);
		}
	}
	pdisplay_render(pdisplay);
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
		if (pdisplay_add_card(pdisplay2, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n");
			return (1);
		}
	}
	pdisplay_render(pdisplay2);
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
		if (pdisplay_add_card(pdisplay3, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay\n");
			return (1);
		}
	}
	pdisplay_render(pdisplay3);
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
		if (pdisplay_add_card_shed(pdisplay, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay shed\n");
			return (1);
		}
		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pdisplay_add_card_shed(pdisplay2, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pdisplay shed\n");
			return (1);
		}
	}
	pdisplay_render(pdisplay);
	pdisplay_render(pdisplay2);
	notcurses_get_blocking(nc, NULL);
	pdisplay_toggle_display(pdisplay);
	pdisplay_toggle_display(pdisplay2);
	notcurses_get_blocking(nc, NULL);
	pdisplay_clear_screen(pdisplay);
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
	pile_display_render(pile_display);
	// notcurses_get_blocking(nc, NULL);
	for (int i = 0; i < 10; i++)
	{
		t_card_desc	card;

		if (deck_draw_card(deck, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card\n");
			return (1);
		}
		if (pile_display_add_card_top(pile_display, card) != 0)
		{
			dprintf(STDERR_FILENO, "Failed to add card to pile display\n");
			return (1);
		}
	}
	pile_display_render(pile_display);
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
	struct s_deck_display	*deck_display;

	deck_display_create(&deck_display, notcurses_stdplane(nc), deck, 0, 0);
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
	hand_calculate_max_displayable(notcurses_stdplane(nc), hand);
	pdisplay_create(&pdisplay, notcurses_stdplane(nc), PDISPLAY_ORIENTATION_RIGHT);
	pdisplay_calculate_max_displayable(notcurses_stdplane(nc), pdisplay);
	pdisplay_show_shed(pdisplay);
	hand_show_shed(hand);
	deck_display_render(deck_display);
	deck_display_draw_to_shed(deck_display, false, hand, 6);
	deck_display_draw_to_shed(deck_display, true, pdisplay, 6);
	pile_display_render(pile_display);
	pdisplay_render(pdisplay);
	hand_render(hand);
	deck_display_render(deck_display);
	notcurses_render(nc);
	sleep(1);
	// Then we give each player 5 cards in their hand
	// // TODO: draw to hand
	deck_display_draw_to_hand(deck_display, false, hand, 5);
	deck_display_draw_to_hand(deck_display, true, pdisplay, 5);
	pdisplay_show_hand(pdisplay);
	hand_show_hand(hand);
	deck_display_render(deck_display);
	pile_display_render(pile_display);
	pdisplay_render(pdisplay);
	hand_render(hand);
	deck_display_render(deck_display);
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
		else if (key == NCKEY_REFRESH || key == NCKEY_BACKSPACE)
			notcurses_refresh(nc, NULL, NULL);
		else if (key == NCKEY_SPACE)
		{
			hand_toggle_display(hand);
			pdisplay_toggle_display(pdisplay);
		}
		else if (key == NCKEY_ENTER || key == '\n' || key == '\r')
		{
			int	has_old = 0;
			t_card_desc	old_top;
			t_card_desc	selected_card;
			has_old = !pile_top_card(pile_display, &old_top);
			if (hand_pop_selected_card(hand, &selected_card))
			{
				dprintf(STDERR_FILENO, "Failed to pop card from hand\n");
			}
			pile_display_add_card_top(pile_display, selected_card);
			if (deck_display->deck->remaining && hand->card_count < 5)
				deck_display_draw_to_hand(deck_display, false, hand, 1);
			if (has_old && old_top.rank > selected_card.rank)
				pile_display_return_to_hand(pile_display, false, hand);
			int	ok, swap;
			has_old = !pile_top_card(pile_display, &old_top);
			if (pdisplay->card_count)
				{
				swap = pdisplay_show_hand(pdisplay);
				ok = pdisplay_pop_card_index(pdisplay, 0, &selected_card);
				}
			else if (pdisplay->shed_count)
				{
				swap = pdisplay_show_shed(pdisplay);
				ok = pdisplay_pop_card_index(pdisplay, pdisplay->shed_count % 6, &selected_card);
				}
			if (!swap)
					pdisplay_toggle_display(pdisplay);
			if (ok == 0)
				pile_display_add_card_top(pile_display, selected_card);
			if (deck_display->deck->remaining && pdisplay->card_count < 5)
				deck_display_draw_to_hand(deck_display, true, pdisplay, 1);
			if (has_old && old_top.rank > selected_card.rank)
				pile_display_return_to_hand(pile_display, true, pdisplay);
		}
		else
			dprintf(STDERR_FILENO, "Pressed key: %d\n", key);
		pile_display_render(pile_display);
		pdisplay_render(pdisplay);
		hand_render(hand);
		deck_display_render(deck_display);
		notcurses_render(nc) ;
	}
	/*
	struct s_pdisplay	*pdisplay;
	struct s_pile_display	*pile_display;
	unsigned int		width, height;
	struct s_deck_display	*deck_display;
	*/
	pdisplay_destroy(pdisplay);
	pile_display_destroy(pile_display);
	deck_display_destroy(deck_display);
	return (0);
}

int	test_deck_display_anon(struct notcurses *nc)
{
	struct s_deck_anon	*d;

	if (deck_anon_init(&d, notcurses_stdplane(nc), 0, 0, 0))
	{
		dprintf(STDERR_FILENO, "Failed to init anon deck\n");
		return (1);
	}
	deck_anon_render(nc, d);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	for (int i = 0; i < 6; i++)
		deck_anon_inc(d);
	deck_anon_render(nc, d);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	for (int i = 0; i < 3; i++)
		deck_anon_dec(d);
	deck_anon_render(nc, d);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	deck_anon_sub(d, 100);
	deck_anon_render(nc, d);
	notcurses_render(nc);
	notcurses_get_blocking(nc, NULL);
	deck_anon_destroy(d);
	return (0);
}

int	test_deck_display(
	struct notcurses *nc,
	struct s_deck *deck,
	struct s_hand *hand
)
{
	struct s_deck_display *deck_display;

	deck_display_create(&deck_display, notcurses_stdplane(nc), deck, 0, 0);
	deck_display_render(deck_display);
	hand_render(hand);
	notcurses_render(nc);
	for (int i = 0; i < 10; i++)
	{
		struct s_card_desc	card;
		if (deck_display_draw_top_card(deck_display, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card from the deck display\n");
			return (1);
		}
		if (hand_add_card(hand, card))
		{
			dprintf(STDERR_FILENO, "Failed to add a card to the hand\n");
			return (1);
		}
		deck_display_render(deck_display);
		hand_render(hand);
		notcurses_render(nc);
		notcurses_get_blocking(nc, NULL);
	}
	for (int i = 0; i < 10; i++)
	{
		struct s_card_desc	card;
		hand->card_selected[0] = 0;
		if (hand_pop_selected_card(hand, &card))
		{
			dprintf(STDERR_FILENO, "Failed to pop card from the hand\n");
			return (1);
		}
		deck_return_card(deck, card);
		deck_display->is_dirty = 1;
		deck_display_render(deck_display);
		hand_render(hand);
		notcurses_render(nc);
		notcurses_get_blocking(nc, NULL);
	}
	deck_display_destroy(deck_display);
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
	// if (test_deck_display(nc, deck, hand))
	// {
	// 	dprintf(STDERR_FILENO, "Failed test deck display\n");
	// 	notcurses_stop(nc);
	// 	return (1);
	// }
	if (putting_it_all_together(nc, hand, deck) != 0)
	{
		dprintf(STDERR_FILENO, "Putting it all together test failed\n");
		// cleanup_client(nc, deck, hand);
		notcurses_stop(nc);
		return (1);
	}
	// if (test_deck_display_anon(nc))
	// {
	// 	dprintf(STDERR_FILENO, "Problem occured testing the deck anon\n");
	// 	notcurses_stop(nc);
	// 	return (1);
	// }
	hand_destroy(hand);
	deck_destroy(deck);
	destroy_cards();
	notcurses_stop(nc);
	return (0);
}
