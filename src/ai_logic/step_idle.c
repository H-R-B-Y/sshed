
# include "game/game_data.h"
# include "game/ai_state_machine.h"

int	ai_step_idle(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
	(void)pile;(void)hand;
	data->progress = 0;
	data->state = AI_STATE_THINKING;
	return (0);
}

