#include "card_plane.h"

t_free_list	*card_plane_allocator = NULL;

__attribute__((constructor))
void			init_card_plane_allocator(void)
{
	card_plane_allocator = (t_free_list *)malloc(sizeof(t_free_list));
	init_free_list(card_plane_allocator, CARD_PLANE_ALLOCATOR_SIZE);
	if (card_plane_allocator->arena == NULL)
	{
		dprintf(STDERR_FILENO, "Failed to initialize card plane allocator\n");
		free(card_plane_allocator);
		card_plane_allocator = NULL;
	}
}

__attribute__((destructor))
void			destroy_card_plane_allocator(void)
{
	if (card_plane_allocator)
	{
		destroy_free_list(card_plane_allocator);
		free(card_plane_allocator);
		card_plane_allocator = NULL;
	}
}

struct s_card_plane *card_plane_create(
	t_card_desc *card_desc
)
{
	struct s_card_plane	*card_plane;

	if (!card_desc)
		return (NULL);
	if (card_plane_allocator)
		card_plane = checkout_free_list(card_plane_allocator, sizeof(struct s_card_plane));
	else
		card_plane = malloc(sizeof(struct s_card_plane));
	if (!card_plane)
		return (NULL);
	card_plane->orientation = CARD_ORIENTATION_VERTICAL;
	card_plane->last_orientation = CARD_ORIENTATION_VERTICAL;
	card_plane->card_desc = card_desc;
	card_plane->plane = 0;
	card_plane->plane_shown = 0;
	return (card_plane);
}

void	card_plane_destroy(struct s_card_plane *card_plane)
{
	if (!card_plane)
		return;
	if (card_plane->plane)
	{
		ncplane_destroy(card_plane->plane);
		card_plane->plane = NULL;
	}
	if (card_plane_allocator)
		return_free_list(card_plane_allocator, card_plane);
	else
		free(card_plane);
	return ;
}
