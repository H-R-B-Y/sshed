
#include "hand.h"

int	hand_create(struct s_hand **hand, struct ncplane *parent)
{
	struct s_hand	*new_hand;
	unsigned int	width, height;

	if (!hand || !parent)
		return (1);
	new_hand = ft_calloc(1, sizeof(struct s_hand));
	if (!new_hand)
		return (1);
	new_hand->hand_dirty = 0;
	ncplane_dim_yx(parent, &height, &width);
	new_hand->hand_plane = ncplane_create(parent, &(struct ncplane_options){
		.y = HAND_AREA_Y(width, height),
		.x = HAND_AREA_X(width, height),
		.rows = HAND_AREA_HEIGHT(width, height),
		.cols = HAND_AREA_WIDTH(width, height),
		.flags = 0,
	});
	make_plane_transparent(new_hand->hand_plane);
	if (!new_hand->hand_plane)
		return (free(new_hand), 1);
	ncplane_set_name(new_hand->hand_plane, "hand_plane");
	new_hand->cards = NULL;
	new_hand->card_count = 0;
	ft_memset(new_hand->card_selected, -1, sizeof(new_hand->card_selected));
	new_hand->status = HAND_DISPLAY_HAND;
	new_hand->last_status = HAND_DISPLAY_HAND;
	new_hand->max_cards_to_display = 6;
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
		card_plane_destroy(card_plane);
		free(current);
		current = next;
	} // This does not free the shed ?
	for (int i = 0; i < 6; i++)
	{
		if (!hand->shed[i])
			continue ;
		card_plane = hand->shed[i];
		card_plane_destroy(card_plane);
	}
	if (hand->hand_plane)
		ncplane_destroy(hand->hand_plane);
	free(hand);
}

