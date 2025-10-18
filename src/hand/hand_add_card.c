
#include "hand.h"

# define DEFAULT_PLANE_OPTS \
	{ \
		.y = 0, \
		.x = 0, \
		.rows = 7, /* Assuming card height is 7 */ \
		.cols = 9, /* Assuming card width is 9 */ \
		.flags = 0, \
	}

int hand_add_card(struct s_hand *hand, t_card_desc *card_desc)
{
	struct s_card_plane	*card_plane;
	struct ncplane		*card_ncplane;
	struct s_card		*card;
	t_list				*card_list_node;

	if (!hand || !card_desc)
		return (1);
	card_plane = checkout_free_list(&hand->allocator, sizeof(struct s_card_plane));
	if (!card_plane)
		return (1);
	card_plane->card_desc = card_desc;
	card = hm_get_value(&cardmap, card_desc);
	if (!card)
	{
		return_free_list(&hand->allocator, card_plane);
		return (1);
	}
	card_ncplane = ncplane_create(hand->hand_plane, &(struct ncplane_options)DEFAULT_PLANE_OPTS);
	if (!card_ncplane)
	{
		return_free_list(&hand->allocator, card_plane);
		return (1);
	}
	card_plane->plane = card_ncplane;
	card_list_node = checkout_free_list(&hand->allocator, sizeof(t_list));
	if (!card_list_node)
	{
		ncplane_destroy(card_ncplane);
		return_free_list(&hand->allocator, card_plane);
		return (1);
	}
	card_list_node->content = card_plane;
	card_list_node->next = NULL;
	ft_lstadd_back(&hand->cards, card_list_node);
	hand->card_count++;
	hand->hand_dirty = 1;
	return (0);
}

// Not the most elegant way to do this, but it works for now
void hand_remove_card(struct s_hand *hand, t_card_desc *card_desc)
{
	for (t_list *current = hand->cards; current != NULL; current = current->next)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_plane->card_desc == card_desc)
		{
			t_list *to_remove = current;
			if (to_remove == hand->cards)
				hand->cards = to_remove->next;
			else
			{
				t_list *prev = hand->cards;
				while (prev->next != to_remove)
					prev = prev->next;
				prev->next = to_remove->next;
			}
			if (card_plane->plane)
				ncplane_destroy(card_plane->plane);
			return_free_list(&hand->allocator, card_plane);
			return_free_list(&hand->allocator, to_remove);
			hand->card_count--;
			hand->hand_dirty = 1;
			return;
		}
	}
	// Card not found in hand
	return ;
}
