# include "server/server.h"

/*

This file defines the hanlders for player actions during the play phase of the game.

*/

// note we will have already validated that the player is in the room and the game has started
// and that it is the players turn


static int	move_play_pile_to_player_hand(
	struct s_game_room *room,
	struct s_player_data *pdata
)
{
	// Move all cards from play pile to player's hand
	for (size_t i = 0; i < room->play_pile_count; i++)
	{
		player_add_card(pdata, room->play_pile[i]);
	}
	room->play_pile_count = 0;
	return (0);
}

int	increment_current_player(
	struct s_game_room *room
)
{
	if (!room)
		return (-1);
	room->current_player_idx = (room->current_player_idx + 1) % room->player_count;
	// we want to trigger a sync upate here#
	game_room_send_state_to_all_players(room);
	return (0);
}

static int	check_player_won(
	struct s_game_room *room,
	struct s_player_data *pdata
)
{
	if (pdata->hand_count == 0 && pdata->shed_card_count == 0)
		return (1);
	return (0);
}

// returns 0 if player does not have all cards or cards are from different locations
// returns 1 if player has all cards in hand
// returns 2 if player has all cards in shed face up
// returns 4 if player has all cards in shed face down
static int	validate_player_has_cards_played(
	struct s_player_data *pdata,
	t_min_card_desc *cards,
	size_t card_count
)
{
	// we need to validate that the player has the cards
	
	t_s8	found_count[card_count];
	t_s8	found_at;

	for (size_t i = 0; i < card_count; i++)
		found_count[i] = 0;
	for (size_t i = 0; i < card_count; i++)
	{
		if (player_card_in_hand(pdata, cards[i]))
		{
			found_count[i] = 1;
			continue ;
		}
		else if (player_card_shed_faceup(pdata, cards[i]))
		{
			found_count[i] = 2;
			continue ;
		}
		else if (player_card_shed_facedown(pdata, cards[i]))
		{
			found_count[i] = 4;
			continue ;
		}
		else
			return (0); // Player does not have this card
	}
	found_at = found_count[0];
	for (size_t i = 1; i < card_count; i++)
	{
		if (found_count[i] != found_at)
			return (-1); // Cards are from different locations
	}
	return (found_at); // Player has all cards
}

static int	move_cards_to_play_pile(
	struct s_game_room *room,
	struct s_player_data *pdata,
	t_min_card_desc *cards,
	size_t card_count,
	int from_location
)
{
	for (size_t i = 0; i < card_count; i++)
	{
		// Remove card from player
		if (from_location == 1) // hand
			player_remove_card(pdata, cards[i]);
		else if (from_location == 2) // shed face up
			player_remove_card_faceup(pdata, cards[i]);
		else if (from_location == 4) // shed face down
			player_remove_card_facedown(pdata, cards[i]);
		// Add card to play pile
		room->play_pile[room->play_pile_count] = cards[i];
		room->play_pile_count++;
	}
	return (0);
}


static int	validate_played_card(
	t_min_card_desc *play_pile,
	size_t play_pile_count,
	t_min_card_desc *cards,
	size_t card_count
)
{
	// For simplicity, we will just allow any play for now
	return (1);
}

static int	card_special_action(
	t_min_card_desc card,
	struct s_game_room *room,
	struct s_player_data *pdata
)
{
	// For simplicity, we will assume no special actions for now
	return (0);
}


static int	handle_play_cards(
	struct s_game_room *room,
	struct s_connection *conn,
	struct s_player_data *pdata,
	struct s_player_action_data *action
)
{
	if (room->current_player_idx != pdata->player_lobby_id)
		return (-1); // Not this player's turn
	// Validate player has the cards played
	int location = validate_player_has_cards_played(
		pdata,
		action->data.play_cards.cards,
		action->data.play_cards.card_count
	);
	if (location == -1)
		return (-1);
	else if (location == 0)
	{
		move_play_pile_to_player_hand(room, pdata);
		increment_current_player(room);
		return (0);
	}
	move_cards_to_play_pile(room, pdata, action->data.play_cards.cards, action->data.play_cards.card_count, location);
	if (validate_played_card(
		room->play_pile,
		room->play_pile_count,
		action->data.play_cards.cards,
		action->data.play_cards.card_count
	))
	{
		// Valid play
		// Check if player has won
		card_special_action(
			action->data.play_cards.cards[0],
			room,
			pdata
		);
		if (check_player_won(room, pdata))
		{
			room->who_won = pdata->player_lobby_id;
			room_set_phase(room, PHASE_GAME_OVER);
			room_handle_phase(room);
			return (0);
		}
		// Advance to next player
		increment_current_player(room);
	}
	else
	{
		// Invalid play, move play pile to player's hand
		move_play_pile_to_player_hand(room, pdata);
		// Advance to next player
		increment_current_player(room);
	}
	return (0);
}

int	handle_player_action_play(
	struct s_game_room *room,
	struct s_connection *conn,
	struct s_player_data *pdata,
	struct s_player_action_data *action
)
{
	switch (action->data_type)
	{
	case PLAYER_ACTION_DATA_PLAY_CARDS:
		/*
		play means validate they are playing cards from the right place
		validate the cards are legal to play
		if so play them and trigger any side effects
		else move play pile to players hand (for failed play)
		*/
		return handle_play_cards(room, conn, pdata, action);

	case PLAYER_ACTION_DATA_SWAP_CARDS:
		/* if they swap in play, just send them and error */
		return (-1);
	
	case PLAYER_ACTION_DATA_DEFAULT:
		/*
		No validation needed, just move the cards from the play to the player's hand
		*/
		move_play_pile_to_player_hand(room, pdata);
		increment_current_player(room);
		return (0);
	
	default:
		return (-1);
	}
	return (0);
}