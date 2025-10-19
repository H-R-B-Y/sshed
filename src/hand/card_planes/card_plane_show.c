
#include "hand.h"

int	show_card(
	struct notcurses *nc,
	struct s_hand *hand,
	struct s_card_plane *card_plane
)
{
	if (!hand || !card_plane)
		return (1);
	if (card_plane->plane_shown)
		return (0); // already shown
	card_plane->plane = ncplane_create(hand->hand_plane, &(struct ncplane_options)DEFAULT_PLANE_OPTS);
	if (!card_plane->plane)
		return (1);
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
		ncvisual_blit(nc, card->graphic, &vopts);
	}
	else
		ncvisual_blit(nc, cardbacks[0], &vopts);
	// notcurses_render(nc);
	hand->hand_dirty = 1;
	return (0);
}

int	hide_card(
	struct notcurses *nc,
	struct s_hand *hand,
	struct s_card_plane *card_plane
)
{
	(void)nc;
	if (!hand || !card_plane || !card_plane->plane)
		return (1);
	if (!card_plane->plane_shown)
		return (0); // already hidden
	ncplane_erase(card_plane->plane);
	ncplane_destroy(card_plane->plane);
	// notcurses_render(nc);
	card_plane->plane = NULL;
	card_plane->plane_shown = 0;
	hand->hand_dirty = 1;
	return (0);
}

int	redisplay_card(
	struct notcurses *nc,
	struct s_hand *hand,
	struct s_card_plane *card_plane
)
{
	int	ret;
	
	if (!hand || !card_plane)
		return (1);
	if (card_plane->plane_shown)
		ret = hide_card(nc, hand, card_plane) || show_card(nc, hand, card_plane);
	else
		ret = show_card(nc, hand, card_plane);
	return (ret);
}
