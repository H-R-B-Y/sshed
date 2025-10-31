
# include "game/game_data.h"
# include "game/ai_state_machine.h"


// Initialise the state data
// set state to idle
int	ai_step_none(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
	(void)pile;
	(void)hand;
	data->progress = 0;
	data->state_frames_left = 10;
	data->state = AI_STATE_IDLE;
	return (0);
}

