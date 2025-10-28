
#ifndef MENU_H
# define MENU_H

# include <notcurses/notcurses.h>
# define FT_INCLUDE_ALL
# include "libft.h"

struct s_menu;

struct s_menu_option
{
	enum e_menu_text_type {
		STATIC_TEXT,
		STATIC_TEXT_FUNCTION,
		DYNAMIC_TEXT_FUNCTION,
	}	text_type;
	union
	{
		const char	*option_text;
		char		*(*get_option_text)(struct s_menu *m);
	};
	int			(*option_action)(struct s_menu *m, struct notcurses *nc);
};

struct s_menu
{
	struct s_menu_option	*options;
	unsigned int			option_count;
	unsigned int			selected_index;
	struct ncplane			*menu_plane;
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

#endif
