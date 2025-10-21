
#include "card_plane.h"

int	show_card_plane(
	struct notcurses *nc,
	struct ncplane *parent,
	struct s_card_plane *card_plane
)
{
	if (!card_plane || !parent)
		return (1);
	if (card_plane->plane_shown)
		return (0); // already shown
	if (card_plane->orientation != card_plane->last_orientation)
	{
		if (card_plane->plane)
		{
			ncplane_destroy(card_plane->plane);
			card_plane->plane = NULL;
		}
		card_plane->last_orientation = card_plane->orientation;
	}
	if (!card_plane->plane)
	{
		card_plane->plane = ncplane_create(parent,
			&(struct ncplane_options){
				.rows = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL) ? CARD_H_HEIGHT : CARD_HEIGHT,
				.cols = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL) ? CARD_H_WIDTH : CARD_WIDTH,
				.y = 0,
				.x = 0,
				.flags = 0,
			});
		if (!card_plane->plane)
			return (1);
	}
	struct ncvisual_options vopts = {
		.n = card_plane->plane,
		.x = 0,
		.y = 0,
		.blitter = NCBLIT_PIXEL,
		.scaling = NCSCALE_STRETCH,
	};
	struct ncvisual	*visual = NULL;
	struct s_card	*card = NULL;
	if (card_plane->is_face_down)
		visual = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL) ? cardbacks[2] : cardbacks[0];
	else
	{
		card = hm_get_value(&cardmap, card_plane->card_desc);
		if (!card || !card->graphic)
			return (1);
		visual = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL) ? card->graphic_h : card->graphic;
	}
	ncvisual_blit(nc, visual, &vopts);
	card_plane->plane_shown = 1;
	return (0);
}

int	hide_card_plane(
	struct notcurses *nc,
	struct ncplane *parent,
	struct s_card_plane *card_plane
)
{
	(void)nc;
	(void)parent;
	if (!parent || !card_plane || !card_plane->plane)
		return (1);
	if (!card_plane->plane_shown)
		return (0); // already hidden
	ncplane_erase(card_plane->plane);
	ncplane_destroy(card_plane->plane);
	card_plane->plane = NULL;
	card_plane->plane_shown = 0;
	return (0);
}

int	redisplay_card(
	struct notcurses *nc,
	struct ncplane *parent,
	struct s_card_plane *card_plane
)
{
	int	ret;
	
	if (!card_plane || !parent)
		return (1);
	if (card_plane->plane_shown)
		ret = hide_card_plane(nc, parent, card_plane) || show_card_plane(nc, parent, card_plane);
	else
		ret = show_card_plane(nc, parent, card_plane);
	return (ret);
}
