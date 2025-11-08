
#include "card_plane.h"

static int	_destroy_plane(
	struct s_card_plane *card_plane
)
{
	if (!card_plane || !card_plane->parent)
		return (1);
	if (!card_plane->plane_shown || card_plane->plane == NULL)
		return (1);
	ncplane_destroy(card_plane->plane);
	card_plane->plane = NULL;
	card_plane->plane_shown = 0;
	return (0);
}

static int	_create_plane(
	struct s_card_plane *card_plane
)
{
	if (!card_plane || !card_plane->parent)
		return (1);
	if (card_plane->plane_shown || card_plane->plane != NULL)
		return (1);
	card_plane->plane = ncplane_create(
		card_plane->parent,
		&(struct ncplane_options){
			.rows = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL)
				? CARD_H_HEIGHT
				: CARD_HEIGHT,
			.cols = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL)
				? CARD_H_WIDTH
				: CARD_WIDTH,
			.y = 0,
			.x = 0,
			.flags = 0,
		}
	);
	make_plane_transparent(card_plane->plane);
	if (!card_plane->plane)
		return (1);
	return (0);
}

static int	_show_visual(
	struct s_card_plane *card_plane
)
{
	struct notcurses		*nc;
	struct ncvisual_options	vopts;
	struct ncvisual			*visual;
	struct s_card			*card;

	if (!card_plane || !card_plane->plane)
		return (1);
	nc = ncplane_notcurses(card_plane->plane);
	vopts = (struct ncvisual_options){
		.n = card_plane->plane,
		.x = 0,
		.y = 0,
		.blitter = NCBLIT_PIXEL,
		.scaling = NCSCALE_NONE,
	};
	if (card_plane->is_face_down)
		visual = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL)
			? cardbacks[2]
			: cardbacks[0];
	else
	{
		card = hm_get_value(&cardmap, &card_plane->card_desc);
		if (!card || !card->graphic)
			return (1);
		visual = (card_plane->orientation == CARD_ORIENTATION_HORIZONTAL)
			? card->graphic_h
			: card->graphic;
	}
	if (!ncvisual_blit(nc, visual, &vopts))
		return (1);
	card_plane->plane_shown = 1;
	return (0);
}

int	show_card_plane(
	struct s_card_plane *card_plane
)
{
	if (!card_plane || !card_plane->parent)
		return (1);
	if (card_plane->plane_shown)
		return (0); // already shown
	if (card_plane->orientation != card_plane->last_orientation)
	{
		_destroy_plane(card_plane);
		card_plane->last_orientation = card_plane->orientation;
	}
	if (!card_plane->plane && _create_plane(card_plane))
		return (1);
	return (_show_visual(card_plane));
}

int	hide_card_plane(
	struct s_card_plane *card_plane
)
{
	if (!card_plane || !card_plane->plane)
		return (1);
	if (!card_plane->plane_shown)
		return (0); // already hidden
	_destroy_plane(card_plane);
	return (0);
}

int	redisplay_card(
	struct s_card_plane *card_plane
)
{
	int						ret;
	
	if (!card_plane)
		return (1);
	if (card_plane->plane_shown)
		; // ret = hide_card_plane(card_plane) || show_card_plane(card_plane);
	else
		ret = show_card_plane(card_plane);
	return (ret);
}
