
#include "cards.h"
#include "card_plane.h"


int	card_in_list(
	struct s_card_desc card_desc,
	t_list *list
)
{
	t_list				*current;
	struct s_card_plane	*plane;

	current = list;
	while (current)
	{
		plane = current->content;
		if (plane)
		{
			if (card_is_equal(plane->card_desc, card_desc))
				return (1);
		}
		current = current->next;
	}
	return (0);
}

int	card_in_array(
	struct s_card_desc card_desc,
	struct s_card_plane **planes,
	size_t card_count
)
{
	struct s_card_plane	*plane;
	size_t				idx;

	idx = 0;
	while (idx < card_count)
	{
		plane = planes[idx];
		if (plane)
		{
			if (card_is_equal(plane->card_desc, card_desc))
				return (1);
		}
		idx++;
	}
	return (0);
}
