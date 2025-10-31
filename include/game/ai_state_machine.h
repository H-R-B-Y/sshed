
#ifndef AI_STATE_MACHINE_H
# define AI_STATE_MACHINE_H

# include "enum_macro.h"
# include "cards.h"
# include "card_plane.h"
# include "pdisplay.h"
# include "pile_display.h"


# define AI_FRAME_TIMEOUT 2

# define AI_STATES_LIST(prefix, func) \
	func(prefix, NONE) \
	func(prefix, IDLE) \
	func(prefix, THINKING) \
	func(prefix, PLAYING) \
	func(prefix, DONE) \
	func(prefix, FUCK_FUCK_FUCK)

ENUM_FACTORY(AI_STATE, ai_state, AI_STATES_LIST);

struct s_ai_data
{
	t_ai_state	state;
	int			state_frames_left;
	int			progress;
	t_card_desc	*next_cards[4];
	t_u8		card_count;
	t_card_desc	*highest;
	t_card_desc	*lowest;
	t_card_desc	*closest_up;
	t_card_desc	*closest_down;
	t_card_desc	*special;
	t_card_desc	*card_to_beat;
	// gotta come up with what the AI needs to store in order to make a descision

};

t_ai_state		ai_step(
	struct s_player_action *action,
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

int	ai_step_idle(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

int	ai_step_none(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

int	ai_step_play(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

int	ai_step_think(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

int	ai_step_done(
	struct s_player_action *action,
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

int	ai_step_error(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

#endif
