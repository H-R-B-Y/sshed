
#include "game/game_data.h"

int	_renderer_update(struct s_game_manager *manager)
{
	size_t				i;
	struct s_renderer	*renderer;

	if (!manager)
		return (1);
	if (!manager->renderer_count)
		return (0);
	i = 0;
	while (i < manager->renderer_count)
	{
		renderer = &(manager->renderers[i]);
		if (renderer->render_fn(renderer->data))
			return (MANAGER_RET_ERR("A renderer has returned an error"));
		i++;
	}
	return (0);
}

static int _event_handler(
	struct s_game_manager *manager,
	struct epoll_event *event
)
{
	struct ncinput	input_event;

	if (!manager || !event)
		return (1);
	if (event->data.fd != manager->reading_fd)
		return (0);
	notcurses_get_blocking(manager->nc, &input_event);
	if (input_event.id == NCKEY_TAB)
		notcurses_refresh(manager->nc, NULL, NULL);
	else if (input_event.id == 'q')
		manager->running = 0;
	else if (manager->stdin_handler)
		return (manager->stdin_handler(manager, input_event));
	return (0);
}

// Runs the game manager loop, does not return until exit
int	game_manager_run(
	struct s_game_manager *manager
)
{
	struct epoll_event	events[10];
	int					nfds;
	uint64_t			expirations;

	if (!manager)
		return (1);
	manager->running = 1;
	timerfd_settime(manager->timer_fd,
		0, &(struct itimerspec){
			.it_interval = { .tv_sec = 0, .tv_nsec = REFRESH_RATE_NS },
			.it_value = { .tv_sec = 0, .tv_nsec = REFRESH_RATE_NS}
		}, NULL
	);
	// _background(manager);
	while (manager->running)
	{
		nfds = epoll_wait( manager->epoll_fd, events, 10, -1);
		if (nfds == -1)
		{
			if (errno == EINTR)
				continue ;
			return (1);
		}
		for (int i = 0; i < nfds; i++)
		{
			if (events[i].data.fd == manager->timer_fd)
			{
				if (manager->next_state != GAME_STATE_NONE
					&& game_manager_change_state(manager) != 0)
				{
						dprintf(STDERR_FILENO, "Error changing game state\n");
						if (manager->errmsg)
							dprintf(STDERR_FILENO, "Error message: %s\n", manager->errmsg);
						return (1);
				}
				read(manager->timer_fd, &expirations, sizeof(uint64_t));
				if (expirations == 0)
					continue ;
				else if (expirations > 1)
				{;}// dprintf(STDERR_FILENO, "Warning: Timer fd expirations: %llu\n", (unsigned long long)expirations);
				_renderer_update(manager);
				notcurses_render(manager->nc);
			}
			else if (manager->reading_fd > 0 && events[i].data.fd == manager->reading_fd)
				_event_handler(manager, &events[i]);
		}
	}
	return (0);
}