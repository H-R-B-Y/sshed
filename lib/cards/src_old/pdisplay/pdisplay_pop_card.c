
# include "pdisplay.h"

/*
How do we want to handle popping a card from the pdisplay?
Ideally we would use either the card_desc 
or an index to identify which card.
Using the index is trickier because we need to know if the index
is for the hand or the shed.
So the card descriptor is better maybe?

Lets write both functions and see
*/

int	_pdisplay_pop_card_hand(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
)
{
	struct s_card_plane	*card_comp = NULL;

	if (pdisplay->card_count == 0)
		return (1);
	card_comp = pdisplay->cards->content;
	for (t_list *current = pdisplay->cards; current != NULL;
		(current = current->next)
	)
	{
		card_comp = current->content;
		if (card_comp && card_is_equal(card_comp->card_desc, card_desc))
		{
			card_plane_destroy(card_comp);
			t_list *prev = pdisplay->cards;
			if (pdisplay->cards == current)
				pdisplay->cards = current->next;
			else
			{
				while (prev->next != current)
					prev = prev->next;
				prev->next = current->next;
			}
			pdisplay->card_count--;
			return (0);
		}
	}
	return (1);
}

int	_pdisplay_pop_card_shed(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
)
{

	unsigned int		idx = 0;

	if (pdisplay->shed_count == 0)
		return (1);
	while (idx < 6)// TODO: update for variable length sheds
	{
		if (pdisplay->shed[idx]
			&& card_is_equal(pdisplay->shed[idx]->card_desc, card_desc))
		{
			card_plane_destroy(pdisplay->shed[idx]);
			pdisplay->shed[idx] = NULL;
			pdisplay->shed_count--;
			return (0);
		}
		idx++;
	}
	return (1);
}

int	pdisplay_pop_card_desc(
	struct s_pdisplay *pdisplay,
	struct s_card_desc card_desc
)
{
	int	status;

	status = _pdisplay_pop_card_hand(pdisplay, card_desc);
	if (status)
		status = _pdisplay_pop_card_shed(pdisplay, card_desc);
	if (status)
		return (1);
	pdisplay->pdisplay_dirty = 1;
	return (0);
}

/*
Limit the index in the shed to < 3 so that we can only pop the
correct card at a given index.
although later if we want a bigger shed then we must update this
TODO: when updating the shed max count logic update this also
*/
int	pdisplay_pop_card_index(
	struct s_pdisplay	*pdisplay,
	int					index,
	struct s_card_desc	*popped_card
)
{
	t_card_desc			popped_card_desc;
	struct s_card_plane	*popped_card_plane = NULL;
	int					current_index = 0;

	if (!pdisplay)
		return (1);
	switch (pdisplay->status)
	{
		case (PDISPLAY_HAND):
			for (t_list *current = pdisplay->cards;
				current != NULL;
				(current = current->next),
				(current_index++)
			)
			{
				if (current_index != index)
					continue;
				popped_card_plane = current->content;
				popped_card_desc = popped_card_plane->card_desc;
				if (current == pdisplay->cards)
					pdisplay->cards = current->next;
				else
				{
					t_list	*prev = pdisplay->cards;
					while (prev->next != current)
						prev = prev->next;
					prev->next = current->next;
				}
				card_plane_destroy(popped_card_plane);
				pdisplay->card_count--;
				free(current);
				pdisplay->pdisplay_dirty = 1;
				(*popped_card) = popped_card_desc;
				return (0);
			}
			break ;
		case (PDISPLAY_SHED):
			int	pop_at;
			if (index >= 3)
				index %= 3;
			pop_at = index;
			if (pdisplay->shed[index + 3])
				pop_at += 3;
			if (pdisplay->shed[pop_at] == NULL)
				break ;
			popped_card_desc = pdisplay->shed[pop_at]->card_desc;
			card_plane_destroy(pdisplay->shed[pop_at]);
			pdisplay->shed[pop_at] = NULL;
			pdisplay->shed_count--;
			pdisplay->pdisplay_dirty = 1;
			(*popped_card) = popped_card_desc;
			return (0);
		default:
			break ;
	}
	return (1);
}