#include "card_plane.h"

struct s_card_plane	*card_plane_create(
	struct ncplane *parent,
	t_card_desc card_desc
)
{
	struct s_card_plane	*card_plane;

	if (!parent)
		return (0);
	card_plane = ft_calloc(1, sizeof(struct s_card_plane));
	if (!card_plane)
		return (NULL);
	card_plane->orientation = CARD_ORIENTATION_VERTICAL;
	card_plane->last_orientation = CARD_ORIENTATION_VERTICAL;
	card_plane->card_desc = card_desc;
	card_plane->plane = 0;
	card_plane->plane_shown = 0;
	card_plane->parent = parent;
	return (card_plane);
}

void	card_plane_destroy(struct s_card_plane *card_plane)
{
	if (!card_plane)
		return;
	if (card_plane->plane)
	{
		ncplane_destroy(card_plane->plane);
		card_plane->plane = NULL;
	}
	free(card_plane);
	return ;
}
