
# include "game/game_data.h"
# include "game/ai_state_machine.h"

// We might need some function to initialise the state machine

// Maybe something to free the state machine data (internal data, computer is not allocated)

// Then the most important function is going to be AI step

t_ai_state		ai_step(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
)
{
	(void)data;
	(void)hand;
	(void)pile;
	dprintf(STDERR_FILENO, "AI STEP CALLED");
	return (AI_STATE_DONE);
}

// But AI step will need its function split out into small step logic
