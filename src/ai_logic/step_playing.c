
# include "game/game_data.h"
# include "game/ai_state_machine.h"

int	ai_step_play(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
// so we should have the data populated on what cards we could play:
/*
	we have:
	highest card
	lowest card
	closest up
	closest down
	special
*/
	int	idx = 0;
	int offset = 0;
	if (pile->cards->count && data->card_to_beat)
	{
		if (data->card_to_beat->rank == RANK_SEVEN)
		{
			if (data->closest_down && data->closest_down->rank <= data->card_to_beat->rank)
			{data->next_cards[0] = data->closest_down;
			data->card_count += 1;
			data->state = AI_STATE_DONE;}
			else if (data->lowest && data->lowest->rank <= data->card_to_beat->rank)
			{data->next_cards[0] = data->lowest;
			data->card_count += 1;
			data->state = AI_STATE_DONE;}
			else if (data->special)
			{data->next_cards[0] = data->special;
			data->card_count += 1;
			data->state = AI_STATE_DONE;}
			else
			{
				if (hand->card_count)
					data->next_cards[0] = &((struct s_card_plane *)hand->cards->content)->card_desc;
				else
				{
					if (hand->shed[3] || hand->shed[4] || hand->shed[5])
						offset += 3;
					while (idx < 3)
					{
						if (hand->shed[offset + idx])
						{
							data->next_cards[0] = &hand->shed[offset + idx]->card_desc;
							break ;
						}
						idx++;
					}
				}
				data->card_count += 1;
				data->state = AI_STATE_DONE;
			}
		}
		else
		{
			if (data->closest_up && data->closest_up->rank >= data->card_to_beat->rank)
			{data->next_cards[0] = data->closest_up;
			data->card_count += 1;
			data->state = AI_STATE_DONE;}
			else if (data->highest && data->highest->rank >= data->card_to_beat->rank)
			{data->next_cards[0] = data->highest;
			data->card_count += 1;
			data->state = AI_STATE_DONE;}
			else if (data->special)
			{data->next_cards[0] = data->special;
			data->card_count += 1;
			data->state = AI_STATE_DONE;}
			else
			{
				if (hand->card_count)
					data->next_cards[0] = &((struct s_card_plane *)hand->cards->content)->card_desc;
				else
				{
					if (hand->shed[3] || hand->shed[4] || hand->shed[5])
						offset += 3;
					while (idx < 3)
					{
						if (hand->shed[offset + idx])
						{
							data->next_cards[0] = &hand->shed[offset + idx]->card_desc;
							break ;
						}
						idx++;
					}

				}
				data->card_count += 1;
				data->state = AI_STATE_DONE;
			}
		}
	}
	else
	{
		if (data->lowest)
		{
			data->next_cards[0] = data->lowest;
			data->card_count += 1;
			data->state = AI_STATE_DONE;
		}
		else if (data->closest_up || data->closest_down)
		{
			if (data->closest_up)
				data->next_cards[0] = (data->closest_up);
			else
				data->next_cards[0] = (data->closest_down);
			data->card_count += 1;
			data->state = AI_STATE_DONE;
		}
		else if (data->highest)
		{
			data->next_cards[0] = data->highest;
			data->card_count += 1;
			data->state = AI_STATE_DONE;
		}
		else
		{
			if (hand->card_count)
				data->next_cards[0] = &((struct s_card_plane *)hand->cards->content)->card_desc;
			else
			{
				if (hand->shed[3] || hand->shed[4] || hand->shed[5])
					offset += 3;
				while (idx < 3)
				{
					if (hand->shed[offset + idx])
					{
						data->next_cards[0] = &hand->shed[offset + idx]->card_desc;
						break ;
					}
					idx++;
				}

			}
			data->card_count += 1;
			data->state = AI_STATE_DONE;
		}
	}
	return (0);
}