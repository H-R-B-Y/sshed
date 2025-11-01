
#ifndef AI_STATE_MACHINE_H
# define AI_STATE_MACHINE_H

/*
TODO:
	- More advanced AI logic
		- Consider special cards
		- Consider multiple cards of the same rank
		- Consider bluffing / random behaviour
	- Refactor the state machine to make a cleaner API
		- There should be functions for playing a card from the hand vs a funciton for playing from the shed

*/

# include "enum_macro.h"
# include "cards.h"
# include "card_plane.h"
# include "pdisplay.h"
# include "pile_display.h"

// How many frames does it take for the AI to take a step through its state machine
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
	/// @brief The current state of the AI
	t_ai_state	state;
	/// @brief The number of frames left until the AI does something
	int			state_frames_left;
	/// @brief The progress through the current state
	int			progress;
	/// @brief This will later be used to play multiple cards (TODO:)
	t_card_desc	*next_cards[4];
	/// @brief This will later be used to play multiple cards (TODO:)
	t_u8		card_count;
	/// @brief Highest card in the hand
	t_card_desc	*highest;
	/// @brief Lowest card in the hand
	t_card_desc	*lowest;
	/// @brief The closest card to the last one played (ceiling)
	t_card_desc	*closest_up;
	/// @brief The closest card to the last one played (floor)
	t_card_desc	*closest_down;
	/// @brief First special card in the hand
	t_card_desc	*special;
	/// @brief The card on top of the pile
	/// TODO: This will point to the top card, it should actually account for the invisible cards (8 typically)
	t_card_desc	*card_to_beat;
};

/**
 * @brief Render loop callback to update the AI state machine
 * @param manager The game manager
 * @return 0 on success, 1 on error
 */
int	pre_render_ai_update(
	struct s_game_manager *manager
);

/**
 * @brief Step the AI state machine
 * @param action The action to be filled out by the AI
 * @param data The AI data structure
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return The current state of the AI after the step
 */
t_ai_state		ai_step(
	struct s_player_action *action,
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

/**
 * @brief AI state machine idle step
 * 
 * Idle step, when triggered, just progresses the state machine to the next state.
 * Idle state just means it is waiting for its turn.
 * 
 * @param data The AI data structure
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return 0 on success, 1 on error
 */
int	ai_step_idle(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

/**
 * @brief AI state machine none step
 * 
 * The None step will initialise the AI state machine to the idle state.
 * Any allocation or setup required for the AI should be done here.
 * 
 * @param data The AI data structure
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return 0 on success, 1 on error
 */
int	ai_step_none(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

/**
 * @brief AI state machine play step
 * 
 * This step evaulates the AI's "thoughts" and decides what
 * would be a "good" move to make based on the data populated.
 * 
 * This step probably should take some reference to the game rules
 * because we need to know if there are any special conditions to consider.
 * 
 * The result of this should be that the (first) next_cards is populated
 * and the card_count is set to the number of cards to play (1 for now).
 * 
 * @param data The AI data structure
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return 0 on success, 1 on error
 */
int	ai_step_play(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

/**
 * @brief AI state machine think step
 * 
 * The think step will evaluate the AI's hand and the pile
 * and populate the AI data structure with information about
 * the "best" cards to play.
 * 
 * This function requires multiple calls to complete to emulate
 * thinking time. Each call progresses the internal progress counter.
 * Subsequent calls will perform different parts of the evaluation.
 * The progress counter is reset when the final "thought" is made.
 * 
 * @param data The AI data structure
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return 0 on success, 1 on error
 */
int	ai_step_think(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

/**
 * @brief AI state machine done step
 * 
 * The done step will finalise the AI's action by populating the action
 * structure and setting the action to done.
 * 
 * This signals to the game logic loop that the action is ready to be processed.
 * 
 * @param action The action to be filled out by the AI
 * @param data The AI data structure
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return 0 on success, 1 on error
 */
int	ai_step_done(
	struct s_player_action *action,
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

/**
 * @brief AI state machine error step
 * 
 * The error step is called when something has gone wrong in the AI logic.
 * This function currently just logs an error message.
 * 
 * @param data The AI data structure
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return 0 on success, 1 on error
 */
int	ai_step_error(
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

#endif
