/**
 * @file ai_state_machine.h
 * @brief AI opponent state machine and decision logic
 * 
 * This header defines the AI state machine for computer opponents.
 * The AI operates through a series of states to simulate thinking
 * and decision-making, creating a more natural gameplay experience.
 * 
 * AI States:
 * - NONE: Uninitialized
 * - IDLE: Waiting for turn
 * - THINKING: Analyzing hand and pile
 * - PLAYING: Deciding which card(s) to play
 * - DONE: Action ready for game logic
 * - ERROR: Something went wrong
 * 
 * @todo Improve AI logic:
 * - Consider special cards (2, 3, 7, 10, Jacks)
 * - Play multiple cards of same rank
 * - Add randomness/bluffing behavior
 * - Better API for playing from hand vs shed
 */

#ifndef AI_STATE_MACHINE_H
# define AI_STATE_MACHINE_H

# include "enum_macro.h"
# include "cards.h"
# include "card_plane.h"
# include "pdisplay.h"
# include "pile_display.h"

/* ========================================================================== */
/*                              CONSTANTS                                     */
/* ========================================================================== */

/// @brief Number of frames the AI waits before stepping through states
# define AI_FRAME_TIMEOUT 2

/* ========================================================================== */
/*                         AI STATE ENUMERATION                               */
/* ========================================================================== */

/**
 * @brief AI state machine states
 * 
 * Defines the states the AI can be in:
 * - NONE: Not initialized
 * - IDLE: Waiting (not AI's turn)
 * - THINKING: Evaluating options
 * - PLAYING: Making decision
 * - DONE: Action complete
 * - FUCK_FUCK_FUCK: Error state (colorful name for debugging)
 */
# define AI_STATES_LIST(prefix, func) \
	func(prefix, NONE) \
	func(prefix, IDLE) \
	func(prefix, THINKING) \
	func(prefix, PLAYING) \
	func(prefix, DONE) \
	func(prefix, FUCK_FUCK_FUCK)

ENUM_FACTORY(AI_STATE, ai_state, AI_STATES_LIST);

/* ========================================================================== */
/*                         DATA STRUCTURES                                    */
/* ========================================================================== */

/**
 * @brief AI state machine data structure
 * 
 * Contains all data needed for AI decision-making and state tracking.
 * The AI evaluates its hand and determines the best card to play.
 */
struct s_ai_data
{
	/// @brief Current AI state
	t_ai_state	state;
	/// @brief Frames remaining before next state step
	int			state_frames_left;
	/// @brief Progress through current state (for multi-step states)
	int			progress;

	/* Card selection data */
	/// @brief Cards to play (for future multi-card play support)
	/// @todo Implement multi-card play
	t_card_desc	*next_cards[4];
	/// @brief Number of cards to play
	/// @todo Currently always 1, support multiple
	t_u8		card_count;

	/* Hand analysis results */
	/// @brief Highest rank card in hand
	t_card_desc	*highest;
	/// @brief Lowest rank card in hand
	t_card_desc	*lowest;
	/// @brief Closest card above the pile top
	t_card_desc	*closest_up;
	/// @brief Closest card below the pile top (or equal)
	t_card_desc	*closest_down;
	/// @brief First special card found (2, 3, 7, 10, Jack)
	t_card_desc	*special;

	/// @brief The top card of the pile to beat
	/// @todo Should account for invisible cards (8s typically)
	t_card_desc	*card_to_beat;
};

/* ========================================================================== */
/*                       AI UPDATE AND MAIN STEP                              */
/* ========================================================================== */

// Forward declaration
struct s_game_manager;

/**
 * @brief Render loop callback to update all AI state machines
 * 
 * Called each frame to step through AI logic for all AI players.
 * Manages timing and state transitions for all AIs.
 * 
 * @param manager The game manager
 * @return 0 on success, 1 on error
 */
int	pre_render_ai_update(
	struct s_game_manager *manager
);

/**
 * @brief Step the AI state machine forward
 * 
 * Advances the AI through its state machine based on current state.
 * Calls the appropriate state handler function.
 * 
 * @param manager The game manager
 * @param action The action structure to fill out when ready
 * @param data The AI state machine data
 * @param hand The AI's hand display
 * @param pile The pile display
 * @return Current state after the step
 */
t_ai_state		ai_step(
	struct s_game_manager *manager,
	struct s_player_action *action,
	struct s_ai_data *data,
	struct s_pdisplay *hand,
	struct s_pile_display *pile
);

/* ========================================================================== */
/*                       AI STATE HANDLER FUNCTIONS                           */
/* ========================================================================== */

/**
 * @brief AI none state handler
 * 
 * Initializes the AI state machine from NONE to IDLE.
 * Performs any necessary setup or allocation.
 * 
 * @param data The AI state machine data
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
 * @brief AI idle state handler
 * 
 * Idle state - waiting for the AI's turn.
 * When triggered (turn starts), advances to THINKING state.
 * 
 * @param data The AI state machine data
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
 * @brief AI thinking state handler
 * 
 * Evaluates the AI's hand and the pile to populate decision data.
 * 
 * This is a multi-step state that requires multiple calls to complete,
 * simulating thinking time. Each call progresses the internal progress
 * counter and performs different parts of the evaluation:
 * - Identify highest/lowest cards
 * - Find closest cards to pile top
 * - Identify special cards
 * 
 * @param data The AI state machine data
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
 * @brief AI playing state handler
 * 
 * Evaluates the AI's "thoughts" from the THINKING state and decides
 * which card would be the best move to make.
 * 
 * Uses the populated analysis data (highest, lowest, closest, special)
 * to select a card. Populates next_cards[0] with the chosen card.
 * 
 * @todo Consider special card game rules
 * @todo Support playing multiple cards of same rank
 * 
 * @param data The AI state machine data
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
 * @brief AI done state handler
 * 
 * Finalizes the AI's action by populating the action structure
 * and marking it as ready for the game logic to process.
 * 
 * @param action The action structure to populate
 * @param data The AI state machine data
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
 * @brief AI error state handler
 * 
 * Called when something has gone wrong in the AI logic.
 * Currently just logs an error message.
 * 
 * @param data The AI state machine data
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
