
# include "game/game_data.h"

# ifndef _GNU_SOURCE
#  define _GNU_SOURCE
# endif
# ifndef CLOCK_REALTIME
#  warning "CLOCK_REALTIME not defined, defining to 0 for compatibility"
#  define CLOCK_REALTIME 0
# endif

static int		_game_manager_setup_epoll(struct s_game_manager *manager)
{
	if (!manager)
		return (1);
	manager->epoll_fd = epoll_create1(0);
	if (manager->epoll_fd == -1)
		return (1);
	// Setup timer fd for refreshes
	manager->timer_fd = timerfd_create(CLOCK_REALTIME, 0);
	if (manager->timer_fd == -1)
	{
		close(manager->epoll_fd);
		return (1);
	}
	struct epoll_event	ev;
	ev.events = EPOLLIN;
	ev.data.fd = manager->timer_fd;
	if (epoll_ctl(
			manager->epoll_fd,
			EPOLL_CTL_ADD,
			manager->timer_fd,
			&ev
		) == -1)
	{
		close(manager->timer_fd);
		close(manager->epoll_fd);
		return (1);
	}
	return (0);
}

int		game_manager_init(struct s_game_manager **manager)
{
	if (!manager)
		return (1);
	(*manager) = ft_calloc(1, sizeof(struct s_game_manager));
	if (!(*manager))
		return (1);
	// Initialize other fields as necessary
	(*manager)->state = GAME_STATE_NONE;
	(*manager)->next_state = GAME_STATE_INIT;
	(*manager)->prev_state = GAME_STATE_NONE;
	if (_game_manager_setup_epoll(*manager) != 0)
		return (free(*manager), 1);
	// Initialize renderers cdll
	(*manager)->renderer_count = 0;
	return (0);
}

void	game_manager_destroy(struct s_game_manager *manager)
{
	if (!manager)
		return ;
	// Free state data if necessary
	if (manager->state_data_destructor && manager->state_data)
		manager->state_data_destructor(manager->state_data);
	if (manager->prev_state_data_destructor && manager->prev_state_data)
		manager->prev_state_data_destructor(manager->prev_state_data);
	// Clean up renderers list if necessary
	// cdll_free(&manager->renderers, free);
	// Close epoll and timer fds if necessary
	if (manager->epoll_fd >= 0)
		close(manager->epoll_fd);
	if (manager->timer_fd >= 0)
		close(manager->timer_fd);
	if (manager->nc)
	{
		notcurses_stop(manager->nc);
		destroy_cards();
	}
	free(manager);
}

void	game_manager_stop(
	struct s_game_manager *manager
)
{
	if (!manager)
		return ;
	manager->running = 0;
}
