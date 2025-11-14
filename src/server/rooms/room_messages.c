

# include "server/server.h"

struct s_message	*create_deck_state_message(struct s_game_room *room)
{
	struct s_message		*msg;
	struct s_header_chunk	*header;
	char					*content;
	size_t					offset;
	size_t					content_size;

	if (!room)
		return (NULL);
	// Calculate content size
	content_size = sizeof(t_u32) // number of cards in deck
		+ sizeof(t_u32) // number of cards in play pile
		+ (room->play_pile_count * sizeof(t_min_card_desc)) // play pile cards
		+ sizeof(t_u8) // current phase
		+ sizeof(t_u8); // whose turn it is (lobby id)
	// Allocate message
	msg = ft_calloc(1, sizeof(struct s_message));
	if (!msg)
		return (NULL);
	// Allocate header
	header = ft_calloc(1, sizeof(struct s_header_chunk));
	if (!header)
	{
		free(msg);
		return (NULL);
	}
	// Allocate content
	content = ft_calloc(1, content_size);
	if (!content)
	{
		free(header);
		free(msg);
		return (NULL);
	}
	// Fill content
	offset = 0;
	// number of cards in deck
	*(t_u32 *)(content + offset) = (t_u32)room->deck->remaining;
	offset += sizeof(t_u32);
	// number of cards in play pile
	*(t_u32 *)(content + offset) = (t_u32)room->play_pile_count;
	offset += sizeof(t_u32);
	// play pile cards
	for (size_t i = 0; i < room->play_pile_count; i++)
	{
		*(t_min_card_desc *)(content + offset) = room->play_pile[i];
		offset += sizeof(t_min_card_desc);
	}
	// current phase
	*(t_u8 *)(content + offset) = (t_u8)room->e_phase;
	offset += sizeof(t_u8);
	// whose turn it is (lobby id)
	*(t_u8 *)(content + offset) = room->current_player_idx;
	offset += sizeof(t_u8);
	// Fill header
	header->msg_type = MTYPE_DECK_STATE;
	header->content_length = content_size;
	// Fill message
	msg->header = header;
	msg->content = content;
	return (msg);
}

struct s_message	*create_game_over_message(struct s_game_room *room, t_s8 winner_lobby_id)
{
	struct s_message		*msg;
	struct s_header_chunk	*header;
	char					*content;

	if (!room)
		return (NULL);
	// Allocate message
	msg = ft_calloc(1, sizeof(struct s_message));
	if (!msg)
		return (NULL);
	// Allocate header
	header = ft_calloc(1, sizeof(struct s_header_chunk));
	if (!header)
	{
		free(msg);
		return (NULL);
	}
	// Allocate content
	content = ft_calloc(1, sizeof(t_s8));
	if (!content)
	{
		free(header);
		free(msg);
		return (NULL);
	}
	// Fill content
	*(t_s8 *)content = winner_lobby_id;
	// Fill header
	header->msg_type = MTYPE_GAME_OVER;
	header->content_length = sizeof(t_s8);
	// Fill message
	msg->header = header;
	msg->content = content;
	return (msg);
}
