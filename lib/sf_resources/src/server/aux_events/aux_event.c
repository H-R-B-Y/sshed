

# include "sock_server_int.h"

int	register_aux_event(
	struct s_server *srv,
	int event_fd,
	int events,
	t_event_callback_fn callback,
	void *appdata
)
{
	struct epoll_event	ev;

	if (srv->aux_event_count >= SERVER_MAX_AUX_EVENTS)
		return (-1); // Max aux events reached
	ev.events = events;
	ev.data.fd = event_fd;
	if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_ADD, event_fd, &ev) == -1)
		return (-1); // Failed to add to epoll
	srv->aux_events[srv->aux_event_count].event_fd = event_fd;
	srv->aux_events[srv->aux_event_count].events = events;
	srv->aux_events[srv->aux_event_count].callback = callback;
	srv->aux_events[srv->aux_event_count].appdata = appdata;
	srv->aux_event_count++;
	return (0);
}

int unregister_aux_event(
	struct s_server *srv,
	int event_fd
)
{
	size_t	i;

	for (i = 0; i < srv->aux_event_count; i++)
	{
		if (srv->aux_events[i].event_fd == event_fd)
		{
			epoll_ctl(srv->epoll_fd, EPOLL_CTL_DEL, event_fd, NULL);
			// Shift remaining events down
			close(srv->aux_events[i].event_fd);
			for (size_t j = i; j < srv->aux_event_count - 1; j++)
			{
				srv->aux_events[j] = srv->aux_events[j + 1];
			}
			srv->aux_event_count--;
			return (0);
		}
	}
	return (-1); // Event not found
}

int	handle_aux_events(
	struct s_server *srv,
	struct epoll_event *event_data
)
{
	for (size_t i = 0; i < srv->aux_event_count; i++)
	{
		if (srv->aux_events[i].event_fd == event_data->data.fd)
		{
			return srv->aux_events[i].callback(srv, event_data, srv->aux_events[i].appdata);
		}
	}
	return (-1); // Event not found
}

int	update_aux_event_events(
	struct s_server *srv,
	int event_fd,
	int new_events
)
{
	struct epoll_event	ev;

	for (size_t i = 0; i < srv->aux_event_count; i++)
	{
		if (srv->aux_events[i].event_fd == event_fd)
		{
			ev.events = new_events;
			ev.data.fd = event_fd;
			if (epoll_ctl(srv->epoll_fd, EPOLL_CTL_MOD, event_fd, &ev) == -1)
				return (-1); // Failed to modify epoll
			srv->aux_events[i].events = new_events;
			return (0);
		}
	}
	return (-1); // Event not found
}

int	aux_event_exists(
	struct s_server *srv,
	int event_fd
)
{
	for (size_t i = 0; i < srv->aux_event_count; i++)
	{
		if (srv->aux_events[i].event_fd == event_fd)
			return (1); // Found
	}
	return (0); // Not found
}
