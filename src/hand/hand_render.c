

# include "hand.h"

void	hand_render(struct notcurses *nc, struct s_hand *hand)
{
	t_list				*current;
	struct s_card_plane	*card_plane;
	t_card				*card;
	struct ncvisual_options vopts;
	unsigned int	width, height;
	unsigned int	idx = 0;

	/*
	so we want to move the planes for each card to fill the space
	and for the selected card we want to move it up a bit
	we can do this by iterating over the list of cards and setting their
	positions based on their index in the list
	*/
	// dprintf(STDERR_FILENO, "Rendering hand with %d cards, selected card: %d\n", hand->card_count, hand->card_selected);
	ncplane_dim_yx(hand->hand_plane, &height, &width);
	if (!hand)
		return;
	if (!hand || !hand->hand_dirty)
		return;
	
	// Second pass: render all cards
	current = hand->cards;
	idx = 0;
	while (current)
	{
		card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_plane->plane && card_plane->card_desc)
		{
			card = hm_get_value(&cardmap, card_plane->card_desc);
			if (card && card->graphic)
			{
				vopts = (struct ncvisual_options){
					.n = card_plane->plane,
					.x = 0,
					.y = 0,
					.blitter = NCBLIT_PIXEL,
				};
				ncvisual_blit(nc, card->graphic, &vopts);
			}
			ncplane_move_yx(card_plane->plane,
				(hand->card_selected == (int)idx) ? 0 : 1,
				width / 2 - (hand->card_count * 9) / 2 + (idx * 9) - (idx * 1));
		}
		idx++;
		current = current->next;
	}
	notcurses_render(nc);
	hand->hand_dirty = 0;
}
