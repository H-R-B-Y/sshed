#include "pdisplay.h"

int		pdisplay_create(
	struct s_pdisplay **pdisplay,
	struct ncplane *parent,
	enum e_pdisplay_orientation orientation
)
{
	struct s_pdisplay	*new_pdisplay;
	unsigned int		width, height;

	if (!pdisplay || !parent)
		return (1);
	ncplane_dim_yx(parent, &height, &width);
	new_pdisplay = ft_calloc(1, sizeof(struct s_pdisplay));
	if (!new_pdisplay)
		return (1);
	new_pdisplay->orientation = orientation;
	new_pdisplay->plane = ncplane_create(parent, &(struct ncplane_options){
		.y = PDISPLAY_AREA_Y(orientation, width, height),
		.x = PDISPLAY_AREA_X(orientation, width, height),
		.rows = PDISPLAY_AREA_HEIGHT(orientation, width, height),
		.cols = PDISPLAY_AREA_WIDTH(orientation, width, height),
		.flags = 0,
	});
	if (!new_pdisplay->plane)
		return (free(new_pdisplay), 1);
	make_plane_transparent(new_pdisplay->plane);
	ncplane_set_name(new_pdisplay->plane, pdisplay_orientation_str[orientation]);
	new_pdisplay->cards = NULL;
	new_pdisplay->card_count = 0;
	new_pdisplay->shed_count = 0;
	new_pdisplay->pdisplay_dirty = 0;
	new_pdisplay->max_cards_to_display = (orientation == PDISPLAY_ORIENTATION_TOP)
										? 6
										: 5;
	(*pdisplay) = new_pdisplay;
	return (0);
}

void	pdisplay_destroy(
	struct s_pdisplay *pdisplay
)
{
	if (!pdisplay)
		return ;
	if (pdisplay->plane)
		ncplane_destroy(pdisplay->plane);
	t_list	*current = pdisplay->cards;
	t_list	*next;
	while (current)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		next = current->next;
		card_plane_destroy(card_plane);
		free(current); // TODO: make this use a free list
		current = next;
	}
	for (int i = 0; i < 6; i++)
	{
		struct s_card_plane *card_plane = pdisplay->shed[i];
		if (card_plane)
			card_plane_destroy(card_plane);
	}
	free(pdisplay);
}

