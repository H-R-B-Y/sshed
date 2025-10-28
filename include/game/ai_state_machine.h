
#ifndef AI_STATE_MACHINE_H
# define AI_STATE_MACHINE_H

# include "enum_macro.h"
# include "cards.h"
# include "card_plane.h"
# include "pdisplay.h"
# include "pile_display.h"

# define AI_STATES_LIST(prefix, func) \
	func(prefix, NONE) \
	func(prefix, IDLE) \
	func(prefix, THINKING) \
	func(prefix, ANALYSING) \
	func(prefix, PLAYING) \
	func(prefix, DONE) \
	func(prefix, FUCK_FUCK_FUCK)

ENUM_FACTORY(AI_STATE, ai_state, AI_STATES_LIST);

struct s_ai_data
{
	t_ai_state	state;
	int			state_frames_left;
	int			progress;
	// gotta come up with what the AI needs to store in order to make a descision

};

t_ai_state		ai_step(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

#endif
