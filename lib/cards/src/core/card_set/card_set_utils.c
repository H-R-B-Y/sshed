/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_set_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:27:15 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:30:31 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/card_set.h"

t_returncode	card_set_add_card(
	struct s_card_set *set,
	struct s_card_desc card_desc,
	void *card_user_data
)
{
	if (!set || !set->cards)
		return (RETURN_ERROR);
	if (set->card_count == set->capacity)
		return (RETURN_ERROR);
	// Add card to array
	set->cards[set->card_count] = card_desc;
	// Add card to hashmap
	if (hm_add_value(
		&set->card_map,
		&set->cards[set->card_count - 1],
		card_user_data) != RETURN_OK)
	{
		return (RETURN_FATAL);
	}
	set->card_count++;
	return (RETURN_OK);
}

void			*card_set_get_card_data(
	struct s_card_set *set,
	struct s_card_desc card_desc
)
{
	if (!set)
		return (NULL);
	return (hm_get_value(&set->card_map, &card_desc));
}
