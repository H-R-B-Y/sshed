
# include "server/server.h"

struct s_player_data	*create_player_data(char *name)
{
	struct s_player_data	*pdata;

	pdata = ft_calloc(1, sizeof(struct s_player_data));
	if (!pdata)
		return (NULL);
	pdata->name = ft_strdup(name);
	if (!pdata->name)
	{
		free(pdata);
		return (NULL);
	}
	pdata->player_lobby_id = -1; // Not in a lobby initially
	return (pdata);
}

struct s_player_data	*get_player_data_from_conn(struct s_connection *conn)
{
	return ((struct s_player_data *)conn->user_data);
}

void	free_player_data(struct s_player_data *pdata)
{
	if (!pdata)
		return ;
	if (pdata->name)
	{
		free(pdata->name);
		pdata->name = NULL;
	}
	free(pdata);
}
