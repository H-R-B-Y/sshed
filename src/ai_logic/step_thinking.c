
# include "card_plane.h"
# include "game/game_data.h"
# include "game/ai_state_machine.h"

t_card_desc	*my_lowest_card(
	struct s_pdisplay *hand
)
{
	t_list				*node;
	struct s_card_plane	*plane;
	struct s_card_desc	*this_card;

	struct s_card_desc	*lowest;
	
	lowest = NULL;
	if (hand->card_count)
	{
		node = hand->cards;
		while (node)
		{
			plane = node->content;
			this_card = &plane->card_desc;
			if (!lowest || lowest->rank > this_card->rank)
				lowest = this_card;
			node = node->next;
		}
	}
	else
	{
		int	idx;
		int	offset = 0;
		if (hand->shed[3] || hand->shed[3] || hand->shed[5])
			offset += 3;
		idx = 0;
		while (idx < 3)
		{
			if (hand->shed[offset + idx]
				&& (!lowest || hand->shed[offset + idx]->card_desc.rank < lowest->rank))
				lowest = &hand->shed[offset + idx]->card_desc;
			idx++;
		}
	}
	return (lowest);
}

t_card_desc	*my_highest_card(
	struct s_pdisplay *hand
)
{
	t_list				*node;
	struct s_card_plane	*plane;
	struct s_card_desc	*this_card;
	struct s_card_desc	*highest;
	
	highest = NULL;
	if (hand->card_count)
	{
		node = hand->cards;
		while (node)
		{
			plane = node->content;
			this_card = &plane->card_desc;
			if (!highest || highest->rank < this_card->rank)
				highest = this_card;
			node = node->next;
		}
	}
	else
	{
		int	idx;
		int	offset = 0;
		if (hand->shed[3] || hand->shed[3] || hand->shed[5])
			offset += 3;
		idx = 0;
		while (idx < 3)
		{
			if (hand->shed[offset + idx]
				&& (!highest || hand->shed[offset + idx]->card_desc.rank > highest->rank))
				highest = &hand->shed[offset + idx]->card_desc;
			idx++;
		}
	}
	return (highest);
}

t_card_desc	*my_closest_card_down(
	struct s_pdisplay *hand,
	struct s_card_desc *top_card
)
{
	t_list				*node;
	struct s_card_plane	*plane;
	struct s_card_desc	*this_card;
	int					diff = 52;
	struct s_card_desc	*closest;
	
	closest = NULL;
	if (!top_card)
		return (NULL);
	if (hand->card_count)
	{
		node = hand->cards;
		while (node)
		{
			plane = node->content;
			this_card = &plane->card_desc;
			// Find cards lower than top_card (going down in rank)
			if (this_card->rank < top_card->rank 
				&& (!closest || diff > (int)top_card->rank - (int)this_card->rank))
			{
				closest = this_card;
				diff = top_card->rank - this_card->rank;
			}
			node = node->next;
		}
	}
	else
	{
		int	idx;
		int	offset = 0;
		if (hand->shed[3] || hand->shed[4] || hand->shed[5])
			offset += 3;
		idx = 0;
		while (idx < 3)
		{
			if (hand->shed[offset + idx]
				&& hand->shed[offset + idx]->card_desc.rank < top_card->rank
				&& (!closest || diff > (int)top_card->rank - (int)hand->shed[offset + idx]->card_desc.rank))
			{
				diff = top_card->rank - hand->shed[offset + idx]->card_desc.rank;
				closest = &hand->shed[offset + idx]->card_desc;
			}
			idx++;
		}
	}
	return (closest);
}

t_card_desc	*my_closest_card_up(
	struct s_pdisplay *hand,
	struct s_card_desc *top_card
)
{
	t_list				*node;
	struct s_card_plane	*plane;
	struct s_card_desc	*this_card;
	int					diff = 52;
	struct s_card_desc	*closest;
	
	closest = NULL;
	if (!top_card)
		return (NULL);
	if (hand->card_count)
	{
		node = hand->cards;
		while (node)
		{
			plane = node->content;
			this_card = &plane->card_desc;
			// Find cards higher than top_card (going up in rank)
			if (this_card->rank > top_card->rank 
				&& (!closest || diff > (int)this_card->rank - (int)top_card->rank))
			{
				closest = this_card;
				diff = this_card->rank - top_card->rank;
			}
			node = node->next;
		}
	}
	else
	{
		int	idx;
		int	offset = 0;
		if (hand->shed[3] || hand->shed[4] || hand->shed[5])
			offset += 3;
		idx = 0;
		while (idx < 3)
		{
			if (hand->shed[offset + idx]
				&& hand->shed[offset + idx]->card_desc.rank > top_card->rank
				&& (!closest || diff > (int)hand->shed[offset + idx]->card_desc.rank - (int)top_card->rank))
			{
				diff = hand->shed[offset + idx]->card_desc.rank - top_card->rank;
				closest = &hand->shed[offset + idx]->card_desc;
			}
			idx++;
		}
	}
	return (closest);
}

t_card_desc	*my_special_card(
	struct s_pdisplay *hand
)
{
	t_list				*node;
	struct s_card_plane	*plane;
	struct s_card_desc	*this_card;
	
	if (hand->card_count)
	{
		node = hand->cards;
		while (node)
		{
			plane = node->content;
			this_card = &plane->card_desc;
			if (this_card->rank == RANK_EIGHT
				|| this_card->rank == RANK_ACE
				|| this_card->rank == RANK_JOKER_BLACK
				|| this_card->rank == RANK_JOKER_RED)
				return (this_card);
			node = node->next;
		}
	}
	else
	{
		int	idx;
		int	offset = 0;
		if (hand->shed[3] || hand->shed[3] || hand->shed[5])
			offset += 3;
		idx = 0;
		while (idx < 3)
		{
			if (hand->shed[offset + idx])
			{
				this_card = &hand->shed[offset + idx]->card_desc;
				if (this_card->rank == RANK_EIGHT
					|| this_card->rank == RANK_ACE
					|| this_card->rank == RANK_JOKER_BLACK
					|| this_card->rank == RANK_JOKER_RED)
					return (this_card);
			}
			idx++;
		}
	}
	return (NULL);
}

// Since there might be rule changes we definitly need to
// keep more data accessible here, but we can do that later


// This just needs to populate the next_cards
// With cards that it thinks it can play
// Then we just need the play step to pick the best one?
int	ai_step_think(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
	struct s_cdll_node	*node;
	struct s_card_plane	*plane;
	size_t				idx;

	switch (data->progress)
	{
		case (0):
			if (hand->card_count == 0)
				pdisplay_show_shed(hand);
			else
				pdisplay_show_hand(hand);
			if (!data->card_to_beat)
			{
				if (pile->cards->count)
				{
					node = pile->cards->head;
					plane = node->data;
					data->card_to_beat = &plane->card_desc;
					idx = 1;
					while (idx < pile->cards->count
						&& data->card_to_beat->rank == RANK_EIGHT)
					{
						node = node->next;
						plane = node->data;
						data->card_to_beat = &plane->card_desc;
						idx++;
					}
				}
				data->progress++;
			}
			return (0); // This should then wait for the frame timeout
			break ;
		case (1):
			data->highest = my_highest_card(hand);
			data->progress++;
			return (0);
			break ;
		case (2):
			data->lowest = my_lowest_card(hand);
			data->progress++;
			return (0);
			break ;
		case (3):
			data->special = my_special_card(hand);
			data->progress++;
			return (0);
			break ;
		case (4):
			data->closest_up = my_closest_card_up(hand, data->card_to_beat);
			data->progress++;
			return (0);
			break ;
		case (5):
			data->closest_down = my_closest_card_down(hand, data->card_to_beat);
			data->progress++;
			return (0);
			break ;
		case (6):
			data->progress = 0;
			data->state = AI_STATE_PLAYING;
			return (0);
			break ;
	}
	return (1);
}
