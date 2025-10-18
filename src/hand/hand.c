
#include "hand.h"

int hand_create(struct s_hand **hand, struct ncplane *parent, int max_width, int y, int x)
{
	struct s_hand	*new_hand;
	// TODO: Figure out how best to handle the size of the cards like a macro maybe?
	int				hand_height = 7 * 2; // Assuming card height is 7
	int				hand_width = max_width;

	if (!hand || !parent)
		return (1);
	new_hand = ft_calloc(1, sizeof(struct s_hand));
	if (!new_hand)
		return (1);
	new_hand->hand_dirty = 0;
	new_hand->hand_plane = ncplane_create(parent, &(struct ncplane_options){
		.y = y,
		.x = x,
		.rows = hand_height,
		.cols = hand_width,
		.flags = 0,
	});
	if (!new_hand->hand_plane)
		return (free(new_hand), 1);
	new_hand->cards = NULL;
	new_hand->card_count = 0;
	new_hand->card_selected = -1;
	if (init_free_list(&new_hand->allocator, 54 * (sizeof(struct s_card_plane) + sizeof(t_list) + sizeof(t_free_list_node))) == RETURN_FATAL)
	{
		ncplane_destroy(new_hand->hand_plane);
		free(new_hand);
		return (1);
	}
	*hand = new_hand;
	return (0);
}

void hand_destroy(struct s_hand *hand)
{
	t_list				*current;
	t_list				*next;
	struct s_card_plane	*card_plane;

	if (!hand)
		return;
	current = hand->cards;
	while (current)
	{
		next = current->next;
		card_plane = (struct s_card_plane *)current->content;
		if (card_plane)
		{
			if (card_plane->plane)
				ncplane_destroy(card_plane->plane);
			return_free_list(&hand->allocator, card_plane);
			current->content = NULL;
		}
		return_free_list(&hand->allocator, current);
		current = next;
	}
	if (hand->hand_plane)
		ncplane_destroy(hand->hand_plane);
	destroy_free_list(&hand->allocator);
	free(hand);
}

