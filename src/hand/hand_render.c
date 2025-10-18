

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
	ncplane_dim_yx(hand->hand_plane, &height, &width);
	if (!hand || !hand->hand_dirty)
		return;
	ncplane_erase(hand->hand_plane);
	current = hand->cards;
	while (current)
	{
		card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_plane->plane && card_plane->card_desc)
		{
			card = hm_get_value(&cardmap, card_plane->card_desc);
			if (!card || !card->graphic)
			{
				current = current->next;
				idx++;
				continue;
			}
			// we want the cards to be spread across the width of the hand plane
			// with a bit of overlap
			// calculate x position
			unsigned int	card_width = 9; // assuming card width is 9
			unsigned int	x_pos;
			if (hand->card_count * (card_width - 2) + 2 <= width)
			{
				x_pos = idx * (card_width - 2);
			}
			else
			{
				// need to scale down the overlap
				unsigned int	overlap = (hand->card_count * card_width - width) / (hand->card_count - 1);
				if (overlap >= card_width)
					overlap = card_width - 1;
				x_pos = idx * (card_width - overlap);
			}
			// calculate y position
			unsigned int	y_pos = 0;
			if ((int)idx == hand->card_selected)
				y_pos = 0; // move up selected card
			else
				y_pos = 2; // normal position

			
			vopts = (struct ncvisual_options){
				.n = card_plane->plane,
				.x = 0,
				.y = 0,
				.blitter = NCBLIT_PIXEL,
				.flags = 0
			};
			if (ncvisual_blit(nc, card->graphic, &vopts) != card_plane->plane)
			{
				// dprintf(STDERR_FILENO, "Failed to blit card graphic\n");
			}
			int actual_y, actual_x;
			ncplane_move_yx(card_plane->plane, y_pos, x_pos);
			ncplane_yx(card_plane->plane, &actual_y, &actual_x);
			dprintf(STDERR_FILENO, "Card %u: requested pos (%u, %u), actual pos (%d, %d)\n",
				idx, y_pos, x_pos, actual_y, actual_x);
			}
			current = current->next;
			idx++;
		}
	notcurses_render(nc);
	hand->hand_dirty = 0;
}
