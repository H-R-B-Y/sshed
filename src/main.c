
#include "game.h"

int	on_connect(struct s_client *client, void *appdata)
{
	(void)client;
	(void)appdata;

	dprintf(STDERR_FILENO, "Client connected\n");
	return (1);
}

int	on_msg_recv(
	struct s_client *client,
	int prechunk,
	struct s_header_chunk *header,
	void *content,
	void *appdata
)
{
	(void)client;
	(void)appdata;
	(void)prechunk;
	(void)header;

	if (content)
		dprintf(STDOUT_FILENO, "MSG: %s\n", (char *)content);
	else
		dprintf(STDOUT_FILENO, "MSG: (NULL)\n");
	return (1);
}

void	on_disconnect(struct s_client *client, void *appdata)
{
	(void)client;
	(void)appdata;
	dprintf(STDERR_FILENO, "Client disconnected\n");
}

int	on_stdin(struct s_client *client, void *appdata)
{
	struct s_header_chunk	header = {0};
	char					*content;
	
	printf("STDOUT\n");
	(void)appdata;
	content = get_next_line(STDIN_FILENO);
	header.content_length = ft_strlen(content);
	header.msg_type = MTYPE_STR;
	client_send_message(client, 1, &header, content);
	return (1);
}

int	callback(struct s_menu *m, struct notcurses *nc)
{
	(void)m;
	(void)nc;
	dprintf(STDOUT_FILENO, "Callback invoked!\n");
	return (0);
}

int main(void)
{
	struct s_game_manager	*manager;
	if (game_manager_init(&manager) != 0)
	{
		dprintf(STDERR_FILENO, "Failed to initialize game manager\n");
		return (1);
	}
	if (game_manager_run(manager) != 0)
	{
		dprintf(STDERR_FILENO, "Error running game manager\n");
		game_manager_destroy(manager);
		return (1);
	}
	game_manager_destroy(manager);
	return (0);
}