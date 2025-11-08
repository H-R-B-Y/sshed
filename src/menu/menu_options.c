
# include "game/menu.h"
# include "card_plane.h"


struct s_menu_option static_menu_option(
	const char *text,
	int (*action)(struct s_menu *m, struct notcurses *nc)
)
{
	struct s_menu_option	option;

	option.text_type = STATIC_TEXT;
	option.option_text = text;
	option.option_action = action;
	option.disabled = 0;
	option.disabled_check = NULL;
	option.user_data = NULL;
	option.on_select = NULL;
	option.on_deselect = NULL;
	return (option);
}

struct s_menu_option dynamic_menu_option(
	t_menu_dynamic_text_fn get_text,
	int (*action)(struct s_menu *m, struct notcurses *nc)
)
{
	struct s_menu_option	option;

	option.text_type = DYNAMIC_TEXT_FUNCTION;
	option.get_option_text = get_text;
	option.option_action = action;
	option.disabled = 0;
	option.disabled_check = NULL;
	option.user_data = NULL;
	option.on_select = NULL;
	option.on_deselect = NULL;
	return (option);
}
