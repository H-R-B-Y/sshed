#include "hand.h"


struct s_card_plane *card_plane_create(
	struct s_hand *hand,
	t_card_desc *card_desc
)
{
	struct s_card_plane	*card_plane;

	if (!hand || !card_desc)
		return (NULL);
	card_plane = checkout_free_list(&hand->allocator, sizeof(struct s_card_plane));
	if (!card_plane)
		return (NULL);
	card_plane->card_desc = card_desc;
	card_plane->plane = 0;
	card_plane->plane_shown = 0;
	return (card_plane);
}


void	card_plane_destroy(struct s_card_plane *card_plane)
{
	if (!card_plane)
		return ;
	if (card_plane->plane && card_plane->plane_shown)
	{
		ncplane_erase(card_plane->plane);
		ncplane_destroy(card_plane->plane);
		card_plane->plane_shown = 0;
	}
	return_free_list(NULL, card_plane);
}
