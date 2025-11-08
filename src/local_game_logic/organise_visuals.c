
# include "game/game_local.h"
# include "game/game_data.h"

int	re_order_visuals(struct s_game_manager *manager, struct s_game_local *game)
{
	unsigned int width, height;

	notcurses_stddim_yx(manager->nc, &height, &width);
	// Ok so we want to position the deck display, the play pile,
	// and if possible in the current window the swap pile

	pile_display_move(game->pile_display, 
		width / 2,
		(height - CARD_HEIGHT) / 2
	);
	deck_display_move(
		game->deck_display,
		(width / 2) - (CARD_WIDTH + 4),
		(height - CARD_HEIGHT) / 2
	);
	if (game->swap_pile)
	{
		pile_display_move(
			game->swap_pile,
			(width - (game->swap_pile->max_stack * CARD_WIDTH)) / 2,
			height - ((CARD_HEIGHT + 2)* 2)
		);
		game->swap_pile->is_dirty = 1;
		game->hand->hand_dirty = 1;
	}
	if (game->swap_menu)
	{
		ncplane_move_yx(
			game->swap_menu->menu_plane,
			(height / 2) - 5,
			((width / 2) - CARD_WIDTH) - 35
		);
	}
	return (0);
}
