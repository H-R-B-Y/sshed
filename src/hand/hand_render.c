

# include "hand.h"


void	render_shed_cards(struct notcurses *nc, struct s_hand *hand)
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
			redisplay_card(nc, hand->hand_plane, hand->shed[idx]);
			ncplane_move_yx(hand->shed[idx]->plane,
				2 - (!!selected), // Same Y level for all cards
				(width / 2 // half the width of the screen
					 - (3 * (CARD_WIDTH + 2)) / 2) // minus half the width of 3 cards
					 + ((idx -1 )* (CARD_WIDTH + 2)) // adjust for size of card
					
			);
		}
		else
		{
			selected = (hand->card_selected[1] == (int)(idx - 3));
			redisplay_card(nc, hand->hand_plane, hand->shed[idx]);
			ncplane_move_yx(hand->shed[idx]->plane,
				1 - (!!selected), // Same Y level for all cards
				(width / 2 // half the width of the screen
					 - (3 * (CARD_WIDTH + 2)) / 2) // minus half the width of 3 cards
					+ ((idx - 4) * (CARD_WIDTH + 2))
			);
		}
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
			redisplay_card(nc, hand->hand_plane, card_plane);
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
	if (hand->status != hand->last_status)
		hand->last_status = hand->status;
	if (!hand->hand_dirty)
		return ;
	switch (hand->status)
	{
		case HAND_DISPLAY_HAND:
			render_hand_cards(nc, hand);
			break ;
		case HAND_DISPLAY_SHED:
			render_shed_cards(nc, hand);
			break ;
		default:
			break ;
	}
	notcurses_render(nc);
}

