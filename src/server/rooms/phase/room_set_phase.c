# include "server/server.h"


int	room_set_phase(
	struct s_game_room *room,
	enum e_game_phase new_phase
)
{
	if (!room)
		return (-1);
	room->e_phase = new_phase;
	return (0);
}

int	room_handle_deal_cards_phase(
	struct s_game_room *room
)
{
	// Shuffle the deck
	// Deal cards to each player
	// Later, we could register timers/events for dealing animations
	// Notify players of new hands
	// Update game state accordingly
	return (0);
}
int	room_handle_swap_phase(
	struct s_game_room *room
)
{
	// Just notify players to swap cards
	return (0);
}
int	room_handle_play_cards_phase(
	struct s_game_room *room
)
{
	// Notify current player it's their turn
	return (0);
}

int	room_handle_game_over_phase(
	struct s_game_room *room
)
{
	// Notify all players of game over and winner
	return (0);
}

int	room_handle_phase(
	struct s_game_room *room
)
{
	if (!room)
		return (-1);
	switch (room->e_phase)
	{
		case PHASE_NONE:
			return (0); // No action needed
		case PHASE_DEAL_CARDS:
			return room_handle_deal_cards_phase(room);
		case PHASE_SWAP:
			return room_handle_swap_phase(room);
		case PHASE_PLAY_CARDS:
			return room_handle_play_cards_phase(room);
		case PHASE_GAME_OVER:
			return room_handle_game_over_phase(room);
		default:
			return (-1); // Unknown phase
	}
	return (0);
}
