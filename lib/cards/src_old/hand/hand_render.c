

# include "hand.h"

// TODO: Later we should pre-calculate this depending on the window size

void	render_shed_cards(struct s_hand *hand)
{
	unsigned int	width, height, selected;
	ncplane_dim_yx(hand->hand_plane, &height, &width);
	for (unsigned int idx = 0; idx < 6; idx++)
	{
		if (hand->shed[idx] == NULL)
			continue ;
		selected = 0;
		if (idx < 3)
		{
			selected = ((hand->card_selected[1] == (int)idx)
				&& (hand->shed[hand->card_selected[1] + 3] == NULL));
			redisplay_card(hand->shed[idx]);
			ncplane_move_yx(hand->shed[idx]->plane,
				2 - (!!selected), // Same Y level for all cards
				(width / 2 // half the width of the screen
					 - (3 * (CARD_WIDTH)) / 2) // minus half the width of 3 cards
					 + ((idx -1 )* (CARD_WIDTH)) // adjust for size of card
					
			);
		}
		else
		{
			selected = (hand->card_selected[1] == (int)(idx - 3));
			redisplay_card(hand->shed[idx]);
			ncplane_move_yx(hand->shed[idx]->plane,
				1 - (!!selected), // Same Y level for all cards
				(width / 2 // half the width of the screen
					 - (3 * (CARD_WIDTH)) / 2) // minus half the width of 3 cards
					+ ((idx - 4) * (CARD_WIDTH))
			);
		}
	}
	hand->hand_dirty = 0;
}

/*
TODO: 
ok so we want to make it so it only renders hand->max_cards_to_display cards,
so we want the selected card to be in the middle of the render unless
its closer to the start or the end.
The way we calculate this is:
- if selected < hand->max_cards_to_display / 2, start at 0
- else if selected > card_count - (hand->max_cards_to_display / 2), start at card_count - hand->max_cards_to_display
- else start at selected - (hand->max_cards_to_display / 2)
Then we render from start to start + hand->max_cards_to_display
*/
void	render_hand_cards(struct s_hand *hand)
{
	t_list				*current;
	struct s_card_plane	*card_plane;
	unsigned int		width, height;
	unsigned int		idx = 0;
	unsigned int		render_position = 0;
	unsigned int		start = 0;
	unsigned int		cards_to_render;

	ncplane_dim_yx(hand->hand_plane, &height, &width);
	current = hand->cards;
	
	// Calculate how many cards we'll actually render
	cards_to_render = (hand->card_count < hand->max_cards_to_display) ? hand->card_count : hand->max_cards_to_display;
	
	// Calculate start position for windowing
	if (hand->card_count <= hand->max_cards_to_display)
		start = 0;
	else if (hand->card_selected[0] < (int)(hand->max_cards_to_display / 2))
		start = 0;
	else if (hand->card_selected[0] >= (int)(hand->card_count - (hand->max_cards_to_display / 2)))
		start = hand->card_count - hand->max_cards_to_display;
	else
		start = hand->card_selected[0] - (hand->max_cards_to_display / 2);
	idx = 0;
	while (current)
	{
		card_plane = (struct s_card_plane *)current->content;
		if (card_plane && (idx < start || idx >= start + hand->max_cards_to_display))
			hide_card_plane(card_plane);
		else if (card_plane)
		{
			redisplay_card(card_plane);
			ncplane_move_yx(card_plane->plane,
				(hand->card_selected[0] == (int)idx) ? 0 : 1,
				(width / 2) - ((cards_to_render * (CARD_WIDTH)) / 2)
				+ (render_position * (CARD_WIDTH)));
			render_position++;
		}
		idx++;
		current = current->next;
	}
	ncplane_erase_region(hand->hand_plane,
			0, (width / 2) + ((cards_to_render * (CARD_WIDTH)) / 2) + 1,
			10, 10
		);
	if (hand->card_count > hand->max_cards_to_display)
	{
		ncplane_printf_yx(
			hand->hand_plane,
			0,
			(width / 2) + ((cards_to_render * (CARD_WIDTH)) / 2) + 1,
			"+%u",
			hand->card_count - hand->max_cards_to_display
		);
	}
	hand->hand_dirty = 0;
}

int	hand_render(struct s_hand *hand)
{
	if (!hand)
		return (1);
	if (hand->status != hand->last_status)
		hand->last_status = hand->status;
	if (!hand->hand_dirty)
		return (0);
	switch (hand->status)
	{
		case HAND_DISPLAY_HAND:
			render_hand_cards(hand);
			break ;
		case HAND_DISPLAY_SHED:
			render_shed_cards(hand);
			break ;
		default:
			break ;
	}
	return (0);
}

