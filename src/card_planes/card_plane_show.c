
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
		if (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL)
		{
			card_plane->plane = ncplane_create(parent,
				&(struct ncplane_options){ // adjust for ratio of pixels
					.rows = CARD_WIDTH, // ratio of pixels is
					.cols = CARD_HEIGHT,
					.y = 0,
					.x = 0,
					.flags = 0,
				});
		}
		else
		{
			card_plane->plane = ncplane_create(parent,
				&(struct ncplane_options){ 
					.rows = CARD_HEIGHT,
					.cols = CARD_WIDTH,
					.y = 0,
					.x = 0,
					.flags = 0,
				});
		}
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
	if (!card_plane->is_face_down)
	{
		struct s_card	*card = hm_get_value(&cardmap, card_plane->card_desc);
		if (!card || !card->graphic)
			return (1);
		if (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL && card->graphic_h)
			ncvisual_blit(nc, card->graphic_h, &vopts);
		else
			ncvisual_blit(nc, card->graphic, &vopts);
	}
	else
	{
		if (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL && cardbacks[2])
			ncvisual_blit(nc, cardbacks[2], &vopts);
		else
			ncvisual_blit(nc, cardbacks[0], &vopts);
	}
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
