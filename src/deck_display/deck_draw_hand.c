
#include "pdisplay.h"
#include "hand.h"
#include "deck_display.h"

int	deck_display_draw_to_hand(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *hand,
	unsigned int count
)
{
	unsigned int	cards_drawn = 0;

	if (!deck_display || !hand || count == 0)
		return (0);
	while (cards_drawn < count)
	{
		struct s_card_desc	card;
		if (deck_display_remaining_cards(deck_display) == 0)
			return (cards_drawn);
		if (deck_display_draw_top_card(deck_display, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw a card to the hand\n");
			return (1);
		}
		switch (is_pdisplay)
		{
			case (true):
			{
				if (pdisplay_add_card(hand, card))
				{
					dprintf(STDERR_FILENO, "Failed to draw card to hand.hand\n");
					return(cards_drawn);
				}
				break ;
			}
			case (false):
			{
				if (hand_add_card(hand, card))
				{
					dprintf(STDERR_FILENO, "Failed to draw card to pdisplay hand\n");
					return (cards_drawn);
				}
				break ;
			}
			default:
			{
				dprintf(STDERR_FILENO, "Neither a hand or a pdisplay\n");
				return (cards_drawn);
			}
		}
		cards_drawn++;
	}
	return (cards_drawn);
}

int deck_display_draw_to_shed(
	struct s_deck_display *deck_display,
	int is_pdisplay,
	void *shed,
	unsigned int count
)
{
	unsigned int	cards_drawn = 0;

	if (!deck_display || !shed)
		return (0);
	while (cards_drawn < count)
	{
		struct s_card_desc	card;
		if (deck_display_remaining_cards(deck_display) == 0)
			return (cards_drawn);
		if (deck_display_draw_top_card(deck_display, &card))
		{
			dprintf(STDERR_FILENO, "Failed to draw to shed\n");
			return (cards_drawn);
		}
		switch (is_pdisplay)
		{
			case (true):
			{
				if (pdisplay_add_card_shed(shed, card))
				{
					dprintf(STDERR_FILENO, "Failed to draw card to shed\n");
					return (cards_drawn);
				}
			}
			case (false):
			{
				if (hand_add_card_to_shed(shed, card))
				{
					dprintf(STDERR_FILENO, "Failed to draw card to shed\n");
					return (cards_drawn);
				}
			}
			default:
			{
				dprintf(STDERR_FILENO, "Neither a hand nor pdisplay\n");
				return (0);
			}
		}
		cards_drawn++;
	}
	return (cards_drawn);
}
