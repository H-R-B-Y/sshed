
# include "game/game_data.h"

int	setup_reading_fd(
	struct s_game_manager *manager
)
{
	struct epoll_event	ev;

	if (!manager)
		return (1);
	manager->reading_fd = notcurses_inputready_fd(manager->nc);
	if (manager->reading_fd == -1)
		return (1);
	ev.events = EPOLLIN;
	ev.data.fd = manager->reading_fd;
	if (epoll_ctl(manager->epoll_fd, EPOLL_CTL_ADD, ev.data.fd, &ev) == -1)
		return (1);
	return (0);
}

static int	_background(struct s_game_manager *manager)
{
	struct ncplane	*stdn;
	unsigned int	width, height;
	struct nccell	c;

	if (!manager)
		return (1);
	stdn = notcurses_stddim_yx(manager->nc, &height, &width);
	nccell_init(&c);
	if (!stdn)
		return (1);
	ncplane_gradient(
		stdn,
		0, 0,
		height, width,
		0, 0,
		NCCHANNEL_INITIALIZER(0, 128, 0),
		NCCHANNEL_INITIALIZER(0, 128, 0),
		NCCHANNEL_INITIALIZER(0, 0, 128),
		NCCHANNEL_INITIALIZER(0, 0, 128)
	);
	return (0);
}

int	load_init_state(
	struct s_game_manager *manager,
	void **state_data
)
{
	if (!manager || !state_data)
		return (1);
	load_free_prev(manager);
	*state_data = NULL;
	manager->state_data_destructor = NULL;
	manager->stdin_handler = NULL; // No stdin handler for init state
	manager->nc = notcurses_init(&(struct notcurses_options){
		.flags = NCOPTION_NO_ALTERNATE_SCREEN,
		.loglevel = NCLOGLEVEL_SILENT,
	}, NULL);
	if (!manager->nc)
		return (1);
	setup_reading_fd(manager);
	init_cards(manager->nc);
	_background(manager);
	manager->next_state = GAME_STATE_MAIN;
	return (0);
}

void	unload_init_state(
	struct s_game_manager *manager,
	void *state_data
)
{
	(void)state_data;
	unload_clean_all(manager);
	return ;
}
