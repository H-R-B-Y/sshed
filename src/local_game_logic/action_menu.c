#include "game/game_data.h"
#include "game/game_local.h"

/*
TODO: The functions for playing multiple cards are currently dont work.
they need to be updated to play the action with the correct number of cards.
Also we need to updaste handle_player_action to handle multiple card plays!!
*/

int	pick_up_pile_action(
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
	action_default_on_cards(&game->player_action);
	action_ready(&game->player_action);
	game_local_select_hand(game);
	game_local_action_menu_cleanup(manager, game);
	return (0);
}

int play_one_card_action(
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
	action_default_on_cards(&game->player_action);
	action_ready(&game->player_action);
	game_local_select_hand(game);
	game_local_action_menu_cleanup(manager, game);
	return (0);
}

int play_two_cards_action(
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
	action_default_on_cards(&game->player_action);
	action_ready(&game->player_action);
	game_local_select_hand(game);
	game_local_action_menu_cleanup(manager, game);
	return (0);
}

int play_three_cards_action(
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
	action_default_on_cards(&game->player_action);
	action_ready(&game->player_action);
	game_local_select_hand(game);
	game_local_action_menu_cleanup(manager, game);
	return (0);
}

int play_four_cards_action(
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
	action_default_on_cards(&game->player_action);
	action_ready(&game->player_action);
	game_local_select_hand(game);
	game_local_action_menu_cleanup(manager, game);
	return (0);
}

int cancel_action(
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
	game_local_select_hand(game);
	game_local_action_menu_cleanup(manager, game);
	return (0);
}

int	count_of_same_rank(
	struct s_game_local *game
)
{
	struct s_card_desc	reference_card;
	t_u8				count;
	size_t				i;
	t_list				*current;

	if (!game || !game->hand || game->hand->card_count == 0)
		return (0);
	reference_card = game->hand->selected_card_plane->card_desc;
	count = 0;
	i = 0;
	current = game->hand->cards;
	while (current)
	{
		struct s_card_plane	*card_plane;
		card_plane = current->content;
		if (card_plane->card_desc.rank == reference_card.rank)
			count++;
		current = current->next;
		i++;
	}
	
	return (count);
}

int	render_player_action_menu(
	struct s_game_local *game
)
{
	struct s_game_manager	*manager;

	if (!game || !game->player_action_menu)
		return (1);
	manager = game->player_action_menu->user_data;
	if (menu_render(game->player_action_menu))
		return (MANAGER_RET_ERR("Unable to render player action menu"));
	return (0);
}

int game_local_action_menu_init(
	struct s_game_manager *manager,
	struct s_game_local *game
)
{

	struct s_menu_option	menu_opts[] = {
		{.text_type = STATIC_TEXT, .option_text = "Pick up pile", .option_action = pick_up_pile_action},
		{.text_type = STATIC_TEXT, .option_text = "Play 1 card", .option_action = play_one_card_action, .disabled = 1},
		{.text_type = STATIC_TEXT, .option_text = "Play 2 cards", .option_action = play_two_cards_action, .disabled = 1},
		{.text_type = STATIC_TEXT, .option_text = "Play 3 cards", .option_action = play_three_cards_action, .disabled = 1},
		{.text_type = STATIC_TEXT, .option_text = "Play 4 cards", .option_action = play_four_cards_action, .disabled = 1},
		{.text_type = STATIC_TEXT, .option_text = "Sort hand", .option_action = NULL},
		{.text_type = STATIC_TEXT, .option_text = "Cancel", .option_action = cancel_action}
	};

	if (!game)
		return (1);
	if (menu_create(&game->player_action_menu,
		notcurses_stdplane(manager->nc),
		menu_opts,
		sizeof(menu_opts) / sizeof(menu_opts[0])))
		return (MANAGER_RET_ERR("Unable to create player action menu"));
	game->player_action_menu->user_data = manager;
	switch (count_of_same_rank(game))
	{
		case (4):
			game->player_action_menu->options[4].disabled = 0;
			// fallthrough
		case (3):
			game->player_action_menu->options[3].disabled = 0;
			// fallthrough
		case (2):
			game->player_action_menu->options[2].disabled = 0;
			// fallthrough
		case (1):
			game->player_action_menu->options[1].disabled = 0;
			break ;
		default:
			break ;
	}
	ncplane_move_yx(game->player_action_menu->menu_plane, // we should move it just next to where the deck is. (check swap menu position)
		5, 5
	);
	manager->renderers[manager->renderer_count].render_fn = (t_renderer_fn)render_player_action_menu;
	manager->renderers[manager->renderer_count].data = game;
	manager->renderer_count++;
	return (0);
}

int game_local_action_menu_cleanup(
	struct s_game_manager *manager,
	struct s_game_local *game
)
{
	if (!game || !game->player_action_menu)
		return (1);
	for (size_t i = 0; i < manager->renderer_count; i++)
	{
		if (manager->renderers[i].render_fn == (t_renderer_fn)render_player_action_menu)
		{
			for (size_t j = i; j < manager->renderer_count - 1; j++)
				manager->renderers[j] = manager->renderers[j + 1];
			manager->renderer_count--;
			break ;
		}
	}
	menu_destroy(game->player_action_menu);
	game->player_action_menu = NULL;
	game_local_select_hand(game);
	return (0);
}

