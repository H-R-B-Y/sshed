

# include "hand.h"


void	render_shed_cards(struct notcurses *nc, struct s_hand *hand)
{
	// So the shed should also be centered, but 3 cards are face down, so
	// we just render the backs of them, and place them underneath the cards that are face up.
	// I think diagonally offsetting the top and bottom cards would look a bit better.

	unsigned int	width, height, selected;
	(void) selected;
	ncplane_dim_yx(hand->hand_plane, &height, &width);
	for (unsigned int idx = 0; idx < 6; idx++)
	{
		selected = 0;
		// int actual_y, actual_x;
		if (idx < 3)
		{
			/*
			selected if selected + 3 is null
			*/
			selected = ((hand->card_selected[1] == (int)idx)
			&& (hand->shed[hand->card_selected[1] + 3] == NULL));
			if (hand->shed[idx] == NULL)
				continue ;
			redisplay_card(nc, hand, hand->shed[idx]);
			ncplane_move_yx(hand->shed[idx]->plane,
				1, // Same Y level for all cards
				(width / 2 // half the width of the screen
					 - (3 * (CARD_WIDTH + 2)) / 2) // minus half the width of 3 cards
					 + (idx * (CARD_WIDTH + 2)) // adjust for size of card
					
			);
			notcurses_refresh(nc, NULL, NULL);
		}
		else
		{
			selected = (hand->card_selected[1] == (int)(idx - 3));
			if (hand->shed[idx] == NULL)
				continue ;
			redisplay_card(nc, hand, hand->shed[idx]);
			ncplane_move_yx(hand->shed[idx]->plane,
				0, // Same Y level for all cards
				(width / 2 // half the width of the screen
					 - (3 * (CARD_WIDTH + 2)) / 2) // minus half the width of 3 cards
					+ ((idx - 3) * (CARD_WIDTH + 2))
			);
			notcurses_refresh(nc, NULL, NULL);
		}
		// debug_print_shed(hand);
		notcurses_render(nc);
		// debug_print_shed(hand);
	}
	hand->hand_dirty = 0;
}

void	render_hand_cards(struct notcurses *nc, struct s_hand *hand)
{
	t_list				*current;
	struct s_card_plane	*card_plane;
	unsigned int	width, height;
	unsigned int	idx = 0;

	ncplane_dim_yx(hand->hand_plane, &height, &width);
	current = hand->cards;
	idx = 0;
	while (current)
	{
		card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_plane->card_desc)
		{
			redisplay_card(nc, hand, card_plane);
			ncplane_move_yx(card_plane->plane,
				(hand->card_selected[0] == (int)idx) ? 0 : 1,
				(width / 2) - ((hand->card_count * 9) / 2)
				+ ((idx - 1) * 9));
		}
		idx++;
		current = current->next;
	}
	hand->hand_dirty = 0;
}

void	hand_render(struct notcurses *nc, struct s_hand *hand)
{
	if (!hand || !nc)
		return ;
	hand_clear_screen(nc, hand);
	if (hand->status != hand->last_status)
		hand->last_status = hand->status;
	if (!hand->hand_dirty)
		return ;
	switch (hand->status)
	{
		case HAND_DISPLAY_HAND:
			if (hand->hand_dirty)
				render_hand_cards(nc, hand);
			break ;
		case HAND_DISPLAY_SHED:
			if (hand->hand_dirty)
				render_shed_cards(nc, hand);
			break ;
		default:
			break ;
	}
	notcurses_refresh(nc, NULL, NULL);
	notcurses_render(nc);
}

