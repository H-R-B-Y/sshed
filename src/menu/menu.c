#include "game/menu.h"

int	menu_create(
	struct s_menu **menu,
	struct ncplane *parent,
	struct s_menu_option *options,
	unsigned int option_count
)
{
	if (!menu || !parent)
		return (1);
	(*menu) = ft_calloc(1, sizeof(struct s_menu));
	if (!(*menu))
		return (1);
	(*menu)->options = ft_memcpy(
		ft_calloc(option_count, sizeof(struct s_menu_option)),
		options,
		option_count * sizeof(struct s_menu_option)
	);
	if (!(*menu)->options)
		return (free(*menu), 1);
	(*menu)->option_count = option_count;
	(*menu)->selected_index = 0;
	(*menu)->menu_plane = ncplane_create(parent, &(struct ncplane_options){
		.y = 0,
		.x = 0,
		.rows = option_count + 2,
		.cols = 30,
		.flags = 0,
	});
	if (!(*menu)->menu_plane)
		return (free(*menu), 1);
	ncplane_set_name((*menu)->menu_plane, "menu_plane");
	(*menu)->is_dirty = 1;
	return (0);
}

void	menu_destroy(
	struct s_menu *menu
)
{
	if (!menu)
		return ;
	if (menu->menu_plane)
		ncplane_destroy(menu->menu_plane);
	if (menu->options)
		free(menu->options);
	free(menu);
}

int	menu_select_next(
	struct s_menu *menu
)
{
	if (!menu)
		return (1);
	menu->selected_index = (menu->selected_index + 1) % menu->option_count;
	menu->is_dirty = 1;
	return (0);
}

/*
#define NCBOXMASK_TOP    0x0001
#define NCBOXMASK_RIGHT  0x0002
#define NCBOXMASK_BOTTOM 0x0004
#define NCBOXMASK_LEFT
*/

int	menu_select_prev(
	struct s_menu *menu
)
{
	if (!menu)
		return (1);
	if (menu->selected_index == 0)
		menu->selected_index = menu->option_count - 1;
	else
		menu->selected_index--;
	menu->is_dirty = 1;
	return (0);
}

int	menu_add_option(
	struct s_menu *menu,
	const char *option_text,
	int (*option_action)(struct s_menu *m, struct notcurses *nc)
)
{
	struct s_menu_option	*new_options;
	unsigned int			new_count;

	if (!menu || !option_text)
		return (1);
	new_count = menu->option_count + 1;
	new_options = ft_memcpy(
		ft_calloc(new_count, sizeof(struct s_menu_option)),
		menu->options,
		menu->option_count * sizeof(struct s_menu_option)
	);
	if (!new_options)
		return (1);
	new_options[menu->option_count].text_type = STATIC_TEXT;
	new_options[menu->option_count].option_text = option_text;
	new_options[menu->option_count].option_action = option_action;
	free(menu->options);
	menu->options = new_options;
	menu->option_count = new_count;
	menu->is_dirty = 1;
	return (0);
}

int	menu_activate_selected(
	struct s_menu *menu
)
{
	struct notcurses	*nc;
	if (!menu)
		return (1);
	nc = ncplane_notcurses(menu->menu_plane);
	if (!nc)
		return (1);
	if (menu->options[menu->selected_index].option_action)
		return (menu->options[menu->selected_index].option_action(menu, nc));
	return (0);
}

int	menu_render(
	struct s_menu *menu
)
{
	unsigned int	i;
	int				ret;
	char			*str;
	int				no_free;

	if (!menu || !menu->is_dirty)
		return (0);
	ncplane_erase(menu->menu_plane);
	no_free = 0;
	ncplane_perimeter_rounded(menu->menu_plane,
		NCSTYLE_NONE, 0, // fuck me it took so much work to figure out what the paremeters of this function mean
		NCBOXMASK_TOP | NCBOXMASK_RIGHT | NCBOXMASK_BOTTOM | NCBOXMASK_LEFT
	);
	/*
	stylemask relates to the NCSTYLE_ enums and can be configured on a per plane or per cell basis 
	the channel relates to NC channels which specify the RGB of a cell
	and the ctlword is the mask for the box sides to represent
	*/
	for (i = 0; i < menu->option_count; i++)
	{
		if (menu->options[i].text_type == STATIC_TEXT)
			str = (char *)menu->options[i].option_text;
		else
			str = menu->options[i].get_option_text(menu);
		if (!str && ++no_free)
			str = "ERR";
		if (i == menu->selected_index)
			// ret = ncplane_printf_aligned(menu->menu_plane, (int)i + 1, NCALIGN_LEFT, "> %s <", str);
			ret = ncplane_printf_yx(menu->menu_plane, i + 1, 1, "> %s <", str);
		else
			// ret = ncplane_printf_aligned(menu->menu_plane, (int)i + 1, NCALIGN_LEFT, "  %s  ", str);
			ret = ncplane_printf_yx(menu->menu_plane, i + 1, 1, "  %s  ", str);
		if (menu->options[i].text_type == DYNAMIC_TEXT_FUNCTION && !no_free)
			free(str);
		if (ret < 0)
			return (1);
	}
	menu->is_dirty = 0;
	return (0);
}

