
#include "hand.h"

void	debug_print_shed(struct s_hand *hand)
{
	int y, x;

	dprintf(STDERR_FILENO, "Shed contents:\n");
	for (unsigned int i = 0; i < hand->shed_count; i++)
	{
		struct s_card_plane *card_plane = hand->shed[i];
		if (card_plane && card_plane->plane)
		{
			ncplane_yx(card_plane->plane, &y, &x);
			t_card_desc *card_desc = card_plane->card_desc;
			dprintf(STDERR_FILENO, "  Slot %u: plane at (%u, %u) with parent %s", i, y, x,
				ncplane_name(ncplane_parent(card_plane->plane))
			);
			if (card_desc)
			{
				dprintf(STDERR_FILENO, " - Card: %s of %s\n",
					rank_str[card_desc->rank],
					suit_str[card_desc->suit]
				);
			}
			else
			{
				dprintf(STDERR_FILENO, " - Card: (null)\n");
			}
		}
		else
		{
			dprintf(STDERR_FILENO, "  Slot %u: (empty)\n", i);
			continue ;
		}
	}
}

