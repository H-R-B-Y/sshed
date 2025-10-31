
# include "game/game_data.h"
# include "game/ai_state_machine.h"


// We might need some function to initialise the state machine

// Maybe something to free the state machine data (internal data, computer is not allocated)

// Then the most important function is going to be AI step

// Just assume for these steps that all the data is valid


int	ai_step_done(
	struct s_player_action *action,
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
	(void)hand;
	(void)pile;
	(*action) = clean_action();
	if (data->card_count == 0)
		action_default_on_cards(action);
	else
		action_play_cards(action, data->next_cards[0], 1);
	action_ready(action);
	data->state = AI_STATE_IDLE;
	data->special = NULL;
	data->lowest = NULL;
	data->highest = NULL;
	data->closest_down = NULL;
	data->closest_up = NULL;
	data->card_to_beat = NULL;
	return (0);
}

int	ai_step_error(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
	(void)hand;
	(void)pile;
	(void)data;
	dprintf(STDERR_FILENO, "ERROR IN AI LOGIC, SOMETHING WENT WRONG");
	return (0);
}


// Really i think  we are going to need a bit more informaton at some
// point but right now we will stick to this
t_ai_state		ai_step(
	struct s_player_action *action,
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
	int	stat = 0;

	if (!action || !data || !hand || !pile)
		return (AI_STATE_FUCK_FUCK_FUCK);
	// dprintf(STDERR_FILENO, "AI STEP CALLED");
	if (data->state == AI_STATE_NONE)
	{
		ai_step_none(data, hand, pile);
	}
	data->state_frames_left--;
	if (data->state_frames_left == 0)
	{
		switch (data->state)
		{
			case (AI_STATE_IDLE):
				stat = ai_step_idle(data, hand, pile);
				data->state_frames_left = AI_FRAME_TIMEOUT;
				break ;
			case (AI_STATE_THINKING):
				stat = ai_step_think(data, hand, pile);
				data->state_frames_left = AI_FRAME_TIMEOUT;
				break ;
			case (AI_STATE_PLAYING):
				stat = ai_step_play(data, hand, pile);
				data->state_frames_left = AI_FRAME_TIMEOUT;
				break ;
			case (AI_STATE_DONE):
				stat = ai_step_done(action, data, hand, pile);
				data->state_frames_left = AI_FRAME_TIMEOUT;
				break ;
			case (AI_STATE_FUCK_FUCK_FUCK):
				stat = ai_step_error(data, hand, pile);
				data->state_frames_left = AI_FRAME_TIMEOUT;
				break ;
			default:
				stat = 1;
				break ;
		}
	}
	if (stat)
		dprintf(STDERR_FILENO, "Uncaught error in ai step\n");
	return (data->state);
}

// But AI step will need its function split out into small step logic
