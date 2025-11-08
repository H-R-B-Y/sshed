
#ifndef MENU_H
# define MENU_H

# include <notcurses/notcurses.h>
# define FT_INCLUDE_ALL
# include "libft.h"

struct s_menu;
struct s_menu_option;

/*
Should make a more advanced option system.
1. options should have an enabled tag (and possible function for checking enabled status)
2. Options should have on_select and on_deselect callbacks for more advanced menus
*/

typedef int		(*t_menu_action_fn)(struct s_menu *m, struct notcurses *nc);
typedef char	*(*t_menu_dynamic_text_fn)(struct s_menu *m, struct s_menu_option *option);
typedef int		(*t_menu_disabled_check)(struct s_menu *m, struct s_menu_option *option);
typedef int		(*t_menu_callback)(struct s_menu *m, struct s_menu_option *option);

struct s_menu_option
{
	enum e_menu_text_type {
		STATIC_TEXT,
		STATIC_TEXT_FUNCTION,
		DYNAMIC_TEXT_FUNCTION,
	}	text_type;
	union
	{
		const char				*option_text;
		t_menu_dynamic_text_fn	get_option_text;
	};
	t_menu_action_fn	option_action;
	int					disabled;
	t_menu_disabled_check	disabled_check;
	void				*user_data;

	t_menu_callback		on_select;
	t_menu_callback		on_deselect;
};

struct s_menu_option static_menu_option(
	const char *text,
	int (*action)(struct s_menu *m, struct notcurses *nc)
);

struct s_menu_option dynamic_menu_option(
	t_menu_dynamic_text_fn get_text,
	int (*action)(struct s_menu *m, struct notcurses *nc)
);

struct s_menu
{
	struct s_menu_option	*options;
	unsigned int			option_count;
	unsigned int			selected_index;
	struct ncplane			*menu_plane;
	t_u8					is_hidden;
	t_u8					is_dirty;
	void					*user_data;
};

int	menu_create(
	struct s_menu **menu,
	struct ncplane *parent,
	struct s_menu_option *options,
	unsigned int option_count
);

void	menu_destroy(
	struct s_menu *menu
);

int	menu_select_next(
	struct s_menu *menu
);

int	menu_select_prev(
	struct s_menu *menu
);

int	menu_activate_selected(
	struct s_menu *menu
);

int	menu_render(
	struct s_menu *menu
);

int	menu_add_option(
	struct s_menu *menu,
	const char *option_text,
	int (*option_action)(struct s_menu *m, struct notcurses *nc)
);

static inline int	_menu_key_handler(
	struct s_menu *menu,
	struct notcurses *nc
)
{
	int	c_code;

	if (!menu)
		return (1);
	c_code = notcurses_get_blocking(nc, NULL);
	// dprintf(STDERR_FILENO, "Menu key handler got code: %d\n", c_code);
	if (c_code == NCKEY_DOWN)
		return (menu_select_next(menu));
	else if (c_code == NCKEY_UP)
		return (menu_select_prev(menu));
	else if (c_code == NCKEY_ENTER || c_code == '\n' || c_code == '\r')
		return (menu_activate_selected(menu));
	return (0);
}

#endif
