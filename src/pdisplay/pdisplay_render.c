
#include "pdisplay.h"

int _pdisplay_render_shed_left_right(
	struct s_pdisplay *pdisplay
)
{
	unsigned int	width, height;
	ncplane_dim_yx(pdisplay->plane, &height, &width);
	for (int idx = 0; idx < 6; idx++)
	{
		if (pdisplay->shed[idx] == NULL)
			continue ;
		if (idx < 3)
		{
			redisplay_card(pdisplay->shed[idx]);
			ncplane_move_yx(pdisplay->shed[idx]->plane,
				(height / 2)
				- (3 * (CARD_H_HEIGHT + 1)) / 2 // total height of all cards
				+ ((idx) * (CARD_H_HEIGHT + 1)), // adjust for size of card
				1
			);
		}
		else
		{
			redisplay_card(pdisplay->shed[idx]);
			ncplane_move_yx(pdisplay->shed[idx]->plane,
				(height / 2)
				- (3 * (CARD_H_HEIGHT + 1)) / 2 // total height of all cards
				+ ((idx - 3) * (CARD_H_HEIGHT + 1)), // adjust for size of card
				3
			);
		}
	}
	return (0);
}

int _pdisplay_render_shed_top(
	struct s_pdisplay *pdisplay
)
{
	unsigned int	width, height;
	ncplane_dim_yx(pdisplay->plane, &height, &width);
	for (int idx = 0; idx < 6; idx++)
	{
		if (pdisplay->shed[idx] == NULL)
			continue ;
		if (idx < 3)
		{
			redisplay_card(pdisplay->shed[idx]);
			ncplane_move_yx(pdisplay->shed[idx]->plane,
				1,
				(width / 2)
				- (3 * (CARD_WIDTH + 2)) / 2 
				+ ((idx - 1) * (CARD_WIDTH + 2)) // adjust for size of card
			);
		}
		else
		{
			redisplay_card(pdisplay->shed[idx]);
			ncplane_move_yx(pdisplay->shed[idx]->plane,
				2,
				(width / 2)
				- (3 * (CARD_WIDTH + 2)) / 2
				+ ((idx - 4) * (CARD_WIDTH + 2)) // adjust for size of card
			);
		}
	}
	return (0);
}

int _pdisplay_render_left_right(
	struct s_pdisplay *pdisplay
)
{
	t_list			*current;
	unsigned int	width, height;
	ncplane_dim_yx(pdisplay->plane, &height, &width);
	unsigned int	idx = 0;
	current = pdisplay->cards;
	while (current)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		if (card_plane)
		{
			redisplay_card(card_plane);
			ncplane_move_yx(card_plane->plane,
				(height / 2)
				- (pdisplay->card_count * (CARD_H_HEIGHT + 1)) / 2 // total height of all cards
				+ ((idx) * (CARD_H_HEIGHT + 1)), // adjust for size of card
				(pdisplay->orientation == PDISPLAY_ORIENTATION_LEFT) ? 1 : 0
			);
			idx++;
		}
		current = current->next;
	}
	return (0);
}

int _pdisplay_render_top(
	struct s_pdisplay *pdisplay
)
{
	t_list			*current;
	unsigned int	width, height;
	ncplane_dim_yx(pdisplay->plane, &height, &width);
	unsigned int	idx = 0;
	current = pdisplay->cards;
	while (current)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		if (card_plane)
		{
			redisplay_card(card_plane);
			ncplane_move_yx(card_plane->plane,
				1, // Y level for all cards
				(width / 2)
				- (pdisplay->card_count * (CARD_WIDTH)) / 2 // total width of all cards
				+ ((idx - 1) * (CARD_WIDTH)) // adjust for size of card
			);
			idx++;
		}
		current = current->next;
	}
	return (0);
}


int		pdisplay_render(
	struct s_pdisplay *pdisplay
)
{
	/*
	Ok so we are going to want to split this out into the 3 orientations
	and render accordingly.
	*/
	if (!pdisplay || !pdisplay->plane)
		return (1);
	if (pdisplay->pdisplay_dirty == 0)
		return (0); // nothing to do
	switch (pdisplay->orientation)
	{
		case PDISPLAY_ORIENTATION_LEFT:
		{
			switch (pdisplay->status)
			{
				case PDISPLAY_HAND:
					_pdisplay_render_left_right(pdisplay);
					break ;
				case PDISPLAY_SHED:
					_pdisplay_render_shed_left_right(pdisplay);
					break ;
				default:
					return (1);
			}
			break ;
		}
		case PDISPLAY_ORIENTATION_RIGHT:
		{
			switch (pdisplay->status)
			{
				case PDISPLAY_HAND:
					_pdisplay_render_left_right(pdisplay);
					break ;
				case PDISPLAY_SHED:
					_pdisplay_render_shed_left_right(pdisplay);
					break ;
				default:
					return (1);
			}
			break ;
		}
		case PDISPLAY_ORIENTATION_TOP:
		{
			switch (pdisplay->status)
			{
				case PDISPLAY_HAND:
					_pdisplay_render_top(pdisplay);
					break ;
				case PDISPLAY_SHED:
					_pdisplay_render_shed_top(pdisplay);
					break ;
				default:
					return (1);
			}
			break;
		}
		default:
			return (1);
	}
	pdisplay->pdisplay_dirty = 0;
	// notcurses_render(nc);
	return (0);
}

int		pdisplay_clear_screen(
	struct s_pdisplay *pdisplay
)
{
	if (!pdisplay || !pdisplay->plane)
		return (1);
	ncplane_erase(pdisplay->plane);
	t_list	*current = pdisplay->cards;
	while (current)
	{
		struct s_card_plane *card_plane = (struct s_card_plane *)current->content;
		if (card_plane && card_plane->plane)
			ncplane_erase(card_plane->plane);
		current = current->next;
	}
	for (int i = 0; i < 6; i++)
	{
		struct s_card_plane *card_plane = pdisplay->shed[i];
		if (card_plane && card_plane->plane)
			ncplane_erase(card_plane->plane);
	}
	// notcurses_render(nc);
	pdisplay->pdisplay_dirty = 1;
	return (0);
}
