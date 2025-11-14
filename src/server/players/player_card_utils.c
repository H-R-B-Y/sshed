
# include "server/server.h"

int	player_add_card(struct s_player_data *pdata, t_min_card_desc card)
{
	if (!pdata)
		return (-1);
	if (pdata->hand_count >= 52)
		return (-1); // Hand full
	pdata->hand[pdata->hand_count] = card;
	pdata->hand_count++;
	return (0);
}

int	player_remove_card(struct s_player_data *pdata, t_min_card_desc card)
{
	size_t	i;
	size_t	j;

	if (!pdata)
		return (-1);
	// Find card in hand
	for (i = 0; i < pdata->hand_count; i++)
	{
		if (pdata->hand[i] == card)
		{
			// Remove card by shifting others down
			for (j = i; j < pdata->hand_count - 1; j++)
			{
				pdata->hand[j] = pdata->hand[j + 1];
			}
			pdata->hand[pdata->hand_count - 1] = 0; // Clear last slot
			pdata->hand_count--;
			return (0); // Success
		}
	}
	return (-1); // Card not found
}

int	player_add_card_faceup(struct s_player_data *pdata, t_min_card_desc card)
{
	if (!pdata)
		return (-1);
	if (pdata->shed_face_up_mask == 0x07) // 111 means all slots full
		return (-1);
	for (t_u8 i = 0; i < 3; i++)
	{
		if (!(pdata->shed_face_up_mask & (1 << i)))
		{
			pdata->shed_face_up[i] = card;
			pdata->shed_face_up_mask |= (1 << i);
			pdata->shed_card_count++;
			return (0); // Success
		}
	}
	return (-1); // No space found
}

int	player_add_card_facedown(struct s_player_data *pdata, t_min_card_desc card)
{
	if (!pdata)
		return (-1);
	if (pdata->shed_face_down_mask == 0x07) // 111 means all slots full
		return (-1);
	for (t_u8 i = 0; i < 3; i++)
	{
		if (!(pdata->shed_face_down_mask & (1 << i)))
		{
			pdata->shed_face_down[i] = card;
			pdata->shed_face_down_mask |= (1 << i);
			pdata->shed_card_count++;
			return (0); // Success
		}
	}
	return (-1); // No space found
}

int	player_remove_card_faceup(struct s_player_data *pdata, t_min_card_desc card)
{
	if (!pdata)
		return (-1);
	for (t_u8 i = 0; i < 3; i++)
	{
		if ((pdata->shed_face_up_mask & (1 << i))
			&& pdata->shed_face_up[i] == card)
		{
			// Remove card
			pdata->shed_face_up[i] = 0;
			pdata->shed_face_up_mask &= ~(1 << i);
			pdata->shed_card_count--;
			return (0); // Success
		}
	}
	return (-1); // Card not found
}


int	player_remove_card_facedown(struct s_player_data *pdata, t_min_card_desc card)
{
	if (!pdata)
		return (-1);
	for (t_u8 i = 0; i < 3; i++)
	{
		if ((pdata->shed_face_down_mask & (1 << i))
			&& pdata->shed_face_down[i] == card)
		{
			// Remove card
			pdata->shed_face_down[i] = 0;
			pdata->shed_face_down_mask &= ~(1 << i);
			pdata->shed_card_count--;
			return (0); // Success
		}
	}
	return (-1); // Card not found
}

int	player_has_card(struct s_player_data *pdata, t_min_card_desc card)
{
	for (size_t i = 0; i < pdata->hand_count; i++)
	{
		if (pdata->hand[i] == card)
			return (1); // Has card
	}
	for (size_t i = 0; i < 3; i++)
	{
		// Check face up shed cards
		if ((pdata->shed_face_up_mask & (1 << i))
			&& pdata->shed_face_up[i] == card)
			return (1); // Has card
		// Check face down shed cards
		if ((pdata->shed_face_down_mask & (1 << i))
			&& pdata->shed_face_down[i] == card)
			return (1); // Has card
	}
	return (0); // Does not have card
}

int	player_has_cards(struct s_player_data *pdata, t_min_card_desc *cards, size_t card_count)
{
	for (size_t i = 0; i < card_count; i++)
	{
		if (!player_has_card(pdata, cards[i]))
			return (0); // Missing a card
	}
	return (1); // Has all cards
}

int	player_card_in_hand(struct s_player_data *pdata, t_min_card_desc card)
{
	for (size_t i = 0; i < pdata->hand_count; i++)
	{
		if (pdata->hand[i] == card)
			return (1); // Card is in hand
	}
	return (0); // Card not in hand
}

int	player_card_shed_faceup(struct s_player_data *pdata, t_min_card_desc card)
{
	return (
		1 * ((pdata->shed_face_up_mask & 0x01) && pdata->shed_face_up[0] == card)
		+ 1 * ((pdata->shed_face_up_mask & 0x02) && pdata->shed_face_up[1] == card)
		+ 1 * ((pdata->shed_face_up_mask & 0x04) && pdata->shed_face_up[2] == card)
	);
}

int	player_card_shed_facedown(struct s_player_data *pdata, t_min_card_desc card)
{
	return (
		1 * ((pdata->shed_face_down_mask & 0x01) && pdata->shed_face_down[0] == card)
		+ 1 * ((pdata->shed_face_down_mask & 0x02) && pdata->shed_face_down[1] == card)
		+ 1 * ((pdata->shed_face_down_mask & 0x04) && pdata->shed_face_down[2] == card)
	);
}
