/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap_phase.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbreeze <hbreeze@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:55:59 by hbreeze           #+#    #+#             */
/*   Updated: 2025/11/08 11:26:25 by hbreeze          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "game/game_local.h"
# include "game/game_data.h"

static int render_swap_phase(struct s_game_local *game)
{
	struct s_game_manager	*manager;

	if (!game || !game->swap_menu || !game->swap_pile)
		return (1);
	manager = game->swap_menu->user_data;
	if (menu_render(game->swap_menu))
		return (MANAGER_RET_ERR("Unable to render swap menu"));
	if (pile_display_render(game->swap_pile))
		return (MANAGER_RET_ERR("Unable to render swap pile"));
	return (0);
}

int swap_cards_action(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	struct s_game_manager	*manager;
	struct s_game_local		*game;
	if (!menu || !nc)
		return (1);
	manager = menu->user_data;
	if (!manager)
		return (1);
	game = (struct s_game_local *)manager->state_data;
	if (!game)
		return (1);
	swap_phase_select_pile(game);
	return (0);
}

int no_swap_action(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	struct s_game_manager	*manager;
	struct s_game_local		*game;
	if (!menu || !nc)
		return (1);
	manager = menu->user_data;
	if (!manager)
		return (1);
	game = (struct s_game_local *)manager->state_data;
	if (!game)
		return (1);
	game->player_action.action = PLAYER_ACTION_DEFAULT;
	game->player_action.ready = 1;
	game->selected_item = SELECTED_ITEM_NONE;
	return (0);
}

int init_swap_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	struct s_menu_option options[] = {
		{.text_type = STATIC_TEXT, .option_text = "Swap Cards", .option_action = swap_cards_action},
		{.text_type = STATIC_TEXT, .option_text = "Ok", .option_action = no_swap_action}
	};
	
	if (!manager || !game)
		return (1);
	if (menu_create(&game->swap_menu, notcurses_stdplane(manager->nc),
		options, sizeof(options) / sizeof(options[0])))
		return (MANAGER_RET_ERR("Unable to create swap menu"));
	game->swap_menu->user_data = manager;
	// Initialize swap pile
	if (!game->swap_pile)
	{
		if (pile_display_create(&game->swap_pile,
			notcurses_stdplane(manager->nc), CARD_WIDTH + 1, CARD_ORIENTATION_VERTICAL, PILE_DISPLAY_HORIZONTAL, 3, 5, 10))
			return (MANAGER_RET_ERR("Unable to create swap pile display"));
		for (int i = 0; i < 3; i++)
		{
			struct s_card_desc card;
			card = game->hand->shed[3 + i]->card_desc;
			hand_remove_card_shed(game->hand, card);
			pile_display_add_card_top(game->swap_pile, card);
		}
	}
	manager->renderers[manager->renderer_count].render_fn = (t_renderer_fn)render_swap_phase;
	manager->renderers[manager->renderer_count].data = game;
	manager->renderer_count++;
	return (0);
}

int unload_swap_phase(struct s_game_manager *manager, struct s_game_local *game)
{
	if (!manager || !game)
		return (1);
	// Remove swap phase renderer
	for (size_t i = 0; i < manager->renderer_count; i++)
	{
		if (manager->renderers[i].render_fn == (t_renderer_fn)render_swap_phase)
		{
			// Shift remaining renderers down
			for (size_t j = i; j < manager->renderer_count - 1; j++)
			{
				manager->renderers[j] = manager->renderers[j + 1];
			}
			manager->renderer_count--;
			break ;
		}
	}
	menu_destroy(game->swap_menu);
	game->swap_menu = NULL;
	pile_display_destroy(game->swap_pile);
	game->swap_pile = NULL;
	return (0);
}

int swap_pile_clear_selection(struct s_game_local *game)
{
	if (!game || !game->swap_pile)
		return (1);
	game->swap_pile->selected = -1;
	game->swap_pile->is_dirty = 1;
	return (0);
}

int swap_phase_select_menu(struct s_game_local *game)
{
	if (!game || !game->swap_menu)
		return (1);
	game->selected_item = SELECTED_ITEM_SWAP_MENU;
	game->swap_menu->is_hidden = 0;
	game->swap_menu->is_dirty = 1;
	game->swap_pile->selected = -1;
	game->swap_pile->is_dirty = 1;
	game->hand->card_selected[0] = -1;
	game->hand->card_selected[1] = -1;
	game->hand->hand_dirty = 1;
	return (0);
}

int swap_phase_select_pile(struct s_game_local *game)
{
	if (!game || !game->swap_pile)
		return (1);
	game->selected_item = SELECTED_ITEM_SWAP_PILE;
	game->swap_menu->is_hidden = 1;
	game->swap_menu->is_dirty = 1;
	game->swap_pile->selected = 0;
	game->swap_pile->is_dirty = 1;
	game->hand->card_selected[0] = -1;
	game->hand->card_selected[1] = -1;
	game->hand->hand_dirty = 1;
	return (0);
}

int swap_phase_select_hand(struct s_game_local *game)
{
	if (!game || !game->hand)
		return (1);
	game->selected_item = SELECTED_ITEM_SWAP_HAND;
	game->swap_menu->is_hidden = 1;
	game->swap_menu->is_dirty = 1;
	game->hand->card_selected[0] = 0;
	game->hand->card_selected[1] = -1;
	game->hand->hand_dirty = 1;
	return (0);
}

