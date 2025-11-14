#include "game/menu.h"
# include "card_plane.h"

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
	if ((*menu)->options[0].on_select)
		(*menu)->options[0].on_select(*menu, &(*menu)->options[0]);
	(*menu)->menu_plane = ncplane_create(parent, &(struct ncplane_options){
		.y = 0,
		.x = 0,
		.rows = option_count + 2,
		.cols = 30,
		.flags = 0,
	});
	make_plane_transparent((*menu)->menu_plane);
	if (!(*menu)->menu_plane)
		return (free(*menu), 1);
	ncplane_set_name((*menu)->menu_plane, "menu_plane");
	(*menu)->is_hidden = 0;
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
	if (menu->options[menu->selected_index].on_deselect)
		menu->options[menu->selected_index].on_deselect(menu, &menu->options[menu->selected_index]);
	menu->selected_index = (menu->selected_index + 1) % menu->option_count;
	while (menu->options[menu->selected_index].disabled)
		menu->selected_index = (menu->selected_index + 1) % menu->option_count;
	if (menu->options[menu->selected_index].on_select)
		menu->options[menu->selected_index].on_select(menu, &menu->options[menu->selected_index]);
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
	if (menu->options[menu->selected_index].on_deselect)
		menu->options[menu->selected_index].on_deselect(menu, &menu->options[menu->selected_index]);
	if (menu->selected_index == 0)
		menu->selected_index = menu->option_count - 1;
	else
		menu->selected_index--;
	while (menu->options[menu->selected_index].disabled)
	{
		if (menu->selected_index == 0)
			menu->selected_index = menu->option_count - 1;
		else
			menu->selected_index--;
	}
	if (menu->options[menu->selected_index].on_select)
		menu->options[menu->selected_index].on_select(menu, &menu->options[menu->selected_index]);
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
	t_u64			channel;
	t_u64			prev_channel;

	if (!menu || !menu->is_dirty)
		return (0);
	ncplane_erase(menu->menu_plane);
	if (menu->is_hidden)
		return (0);
	channel = NCCHANNELS_INITIALIZER(255, 255, 255, 0, 0, 0);
	ncchannels_set_bg_alpha(&channel, NCALPHA_TRANSPARENT);
	no_free = 0;
	ncplane_perimeter_rounded(menu->menu_plane,NCSTYLE_NONE,channel,NCBOXMASK_TOP | NCBOXMASK_RIGHT | NCBOXMASK_BOTTOM | NCBOXMASK_LEFT);
	prev_channel = ncplane_channels(menu->menu_plane);
	for (i = 0; i < menu->option_count; i++)
	{
		if (menu->options[i].text_type == STATIC_TEXT)
			str = (char *)menu->options[i].option_text;
		else
			str = menu->options[i].get_option_text(menu, &menu->options[i]);
		if (!str && ++no_free)
			str = "ERR";
		ncplane_cursor_move_yx(menu->menu_plane, i + 1, 1);
		if (menu->options[i].disabled)
			channel = NCCHANNELS_INITIALIZER(255, 0, 0, 255, 255, 255);
		else
			channel = NCCHANNELS_INITIALIZER(255, 255, 255, 0, 0, 0);
		ncchannels_set_bg_alpha(&channel, NCALPHA_TRANSPARENT);
		ncchannels_set_fg_alpha(&channel, NCALPHA_OPAQUE);
		ncplane_set_channels(menu->menu_plane, channel);
		if (i == menu->selected_index)
			ret = ncplane_printf(menu->menu_plane, "> %s <", str);
		else
			ret = ncplane_printf(menu->menu_plane, "  %s  ", str);
		if (menu->options[i].text_type == DYNAMIC_TEXT_FUNCTION && !no_free)
			free(str);
		if (ret < 0)
			return (1);
	}
	ncplane_set_channels(menu->menu_plane, prev_channel);
	menu->is_dirty = 0;
	return (0);
}

int menu_show(
	struct s_menu *menu
)
{
	if (!menu)
		return (1);
	menu->is_hidden = 0;
	menu->is_dirty = 1;
	return (0);
}

int menu_hide(
	struct s_menu *menu
)
{
	if (!menu)
		return (1);
	ncplane_erase(menu->menu_plane);
	menu->is_hidden = 1;
	menu->is_dirty = 1;
	return (0);
}
