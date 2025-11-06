/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   card_set.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 15:22:19 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/05 15:29:16 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "core/card_set.h"

t_returncode	card_set_init(
	struct s_card_set *set,
	size_t card_count,
	void *set_user_data,
	t_freefn free_set_user_data,
	t_freefn free_card_user_data
)
{
	if (!set)
		return (RETURN_ERROR);
	set->card_count = 0;
	set->capacity = card_count;
	set->cards = ft_calloc(card_count, sizeof(struct s_card_desc));
	if (!set->cards)
		return (RETURN_FATAL);
	set->card_map = hm_create(
		(t_hashfnc)hash_card,
		(t_hashcmp)compare_card,
		card_count * 2
	);
	if (!set->card_map.pairs)
	{
		free(set->cards);
		return (RETURN_FATAL);
	}
	set->set_user_data = set_user_data;
	set->free_set_user_data = free_set_user_data;
	set->free_card_user_data = free_card_user_data;
	return (RETURN_OK);
}

void			card_set_free(
	struct s_card_set *set
)
{
	size_t	i;
	void	*card_data;

	if (!set)
		return ;
	if (set->free_set_user_data && set->set_user_data)
		set->free_set_user_data(set->set_user_data);
	/// TODO: Update hashmap to take a free function for values
	if (set->card_map.pairs)
		hm_destroy(&set->card_map);
	if (set->cards)
		free(set->cards);
}
