
#include "server/server.h"

struct s_message	*create_player_update_message(struct s_game_room *room, struct s_player_data *pdata)
{
	char					*data;
	struct s_header_chunk	header;
	struct s_message		*msg;

	msg = ft_calloc(1, sizeof(struct s_message));
	if (!msg)
		return (NULL);
	// Calculate size needed
	size_t content_size = sizeof(t_s8) /* player lobby id */
		+ sizeof(t_u8) /* hand count */
		+ (pdata->hand_count * sizeof(t_min_card_desc)) /* hand cards */
		+ sizeof(t_u8) /* shed face up mask */
		+ sizeof(t_u8) /* shed face down mask */
		+ (sizeof(t_min_card_desc) * __builtin_popcount(pdata->shed_face_up_mask)); /* face up shed cards */
	data = ft_calloc(1, content_size);
	if (!data)
	{
		free(msg);
		return (NULL);
	}
	size_t offset = 0;
	// Fill data
	*(t_s8 *)(data + offset) = pdata->player_lobby_id;
	offset += sizeof(t_s8);
	*(t_u8 *)(data + offset) = (t_u8)pdata->hand_count;
	offset += sizeof(t_u8);
	ft_memmove(data + offset, pdata->hand, pdata->hand_count * sizeof(t_min_card_desc));
	offset += pdata->hand_count * sizeof(t_min_card_desc);
	*(t_u8 *)(data + offset) = pdata->shed_face_up_mask;
	offset += sizeof(t_u8);
	*(t_u8 *)(data + offset) = pdata->shed_face_down_mask;
	offset += sizeof(t_u8);
	// Add face up shed cards
	for (t_u8 i = 0; i < 3; i++)
	{
		if (pdata->shed_face_up_mask & (1 << i))
		{
			*(t_min_card_desc *)(data + offset) = pdata->shed_face_up[i];
			offset += sizeof(t_min_card_desc);
		}
	}
	// Setup message
	header.msg_type = MTYPE_PLAYER_HAND;
	header.content_length = content_size;
	msg->header = ft_memdup(&header, sizeof(header));
	msg->content = data;
	return (msg);
}


struct s_message	*create_hand_info_message(struct s_game_room *room, struct s_player_data *pdata)
{
	char					*data;
	struct s_header_chunk	header;
	struct s_message		*msg;

	msg = ft_calloc(1, sizeof(struct s_message));
	if (!msg)
		return (NULL);
	// Calculate size needed
	size_t content_size = sizeof(t_s8) /* player lobby id */
		+ sizeof(t_u8) /* hand count */
		+ sizeof(t_u8) /* shed face down count */
		+ sizeof(t_u8) /* shed face up count */
		+ (sizeof(t_min_card_desc) * __builtin_popcount(pdata->shed_face_up_mask)); /* face up shed cards */
	data = ft_calloc(1, content_size);
	if (!data)
	{
		free(msg);
		return (NULL);
	}
	size_t offset = 0;
	// Fill data
	*(t_s8 *)(data + offset) = pdata->player_lobby_id;
	offset += sizeof(t_s8);
	*(t_u8 *)(data + offset) = (t_u8)pdata->hand_count;
	offset += sizeof(t_u8);
	*(t_u8 *)(data + offset) = (t_u8)__builtin_popcount(pdata->shed_face_down_mask);
	offset += sizeof(t_u8);
	*(t_u8 *)(data + offset) = (t_u8)__builtin_popcount(pdata->shed_face_up_mask);
	offset += sizeof(t_u8);
	// Add face up shed cards
	for (t_u8 i = 0; i < 3; i++)
	{
		if (pdata->shed_face_up_mask & (1 << i))
		{
			*(t_min_card_desc *)(data + offset) = pdata->shed_face_up[i];
			offset += sizeof(t_min_card_desc);
		}
	}
	// Setup message
	header.msg_type = MTYPE_HAND_INFO;
	header.content_length = content_size;
	msg->header = ft_memdup(&header, sizeof(header));
	msg->content = data;
	return (msg);
}
