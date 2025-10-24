#include "pdisplay.h"

int	_pdisplay_add_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
)
{
	struct s_card_plane	*card_plane;

	if (!pdisplay)
		return (1);
	if (pdisplay->card_count >= 54) // TODO: figure out the best limit for the number of cards on the display
		return (1); // Display is full
	card_plane = card_plane_create(pdisplay->plane, card_desc);
	if (!card_plane)
		return (1);
	card_plane->is_face_down = 1;
	// Add to pdisplay's card list
	if (pdisplay->orientation == PDISPLAY_ORIENTATION_LEFT
		|| pdisplay->orientation == PDISPLAY_ORIENTATION_RIGHT)
		card_plane->orientation = CARD_ORIENTATION_HORIZONTAL;
	t_list *card_list_node = ft_calloc(1, sizeof(t_list)); // TODO: use free list
	if (!card_list_node)
	{
		card_plane_destroy(card_plane);
		return (1);
	}
	card_list_node->content = card_plane;
	card_list_node->next = NULL;
	ft_lstadd_back(&pdisplay->cards, card_list_node);
	pdisplay->card_count++;
	if (pdisplay->status == PDISPLAY_HAND)
		pdisplay->pdisplay_dirty = 1;
	return (0);
}

int	pdisplay_add_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
)
{
	int	ret;

	ret = _pdisplay_add_card(pdisplay, card_desc);
	pdisplay_render(pdisplay);
	return (ret);
}

int	_pdisplay_remove_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
)
{
	if (!pdisplay)
		return (1);
	t_list	*current = pdisplay->cards;
	t_list	*prev = NULL;
	while (current)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		if (card_plane && ft_memcmp(&card_plane->card_desc, &card_desc, sizeof(card_desc)) == 0)
		{
			// Remove from list
			if (prev)
				prev->next = current->next;
			else
				pdisplay->cards = current->next;
			card_plane_destroy(card_plane);
			free(current); // TODO: use free list
			pdisplay->card_count--;
			if (pdisplay->status == PDISPLAY_HAND)
				pdisplay->pdisplay_dirty = 1;
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1); // Card not found
}

int	pdisplay_remove_card(
	struct s_pdisplay *pdisplay,
	t_card_desc card_desc
)
{
	int	ret;

	ret = _pdisplay_remove_card(pdisplay, card_desc);
	pdisplay_render(pdisplay);
	return (ret);
}
