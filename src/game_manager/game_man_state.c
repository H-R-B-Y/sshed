
#include "game/game_data.h"

int		game_manager_change_state(
	struct s_game_manager *manager
)
{
	if (!manager)
		return (1);
	if (manager->state == manager->next_state
		|| manager->next_state == GAME_STATE_NONE)
		return (0);
	
	// Unload current state
	if (state_handlers[manager->state].unload_fn)
		state_handlers[manager->state].unload_fn(
			manager,
			manager->state_data
		);
	// transition from paused to playing:
	//	- game unload moves data to prev data
	//	- game pause state loads its menu
	//	- game pause moves game state data back to current state data
	//	- game load checks if current data is populated or not
	manager->prev_state = manager->state;
	manager->state = manager->next_state;
	manager->next_state = GAME_STATE_NONE;

	if (state_handlers[manager->state].load_fn)
		return state_handlers[manager->state].load_fn(
			manager,
			&manager->state_data
		);
	return (0);
}
