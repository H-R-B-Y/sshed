
# include "server/server.h"

// this will be called when the client wants to create a new lobby
struct s_game_room	*create_game_room(void)
{
	struct s_game_room	*room;

	room = ft_calloc(1, sizeof(struct s_game_room));
	if (!room)
		return (NULL);
	// Initialize room fields here as needed
	room->who_won = -1; // No winner at start
	room->deck = deck_create(1);
	return (room);
}

void				free_game_room(struct s_game_room *room)
{
	if (!room)
		return ;
	if (room->deck)
	{
		deck_destroy(room->deck);
		room->deck = NULL;	
	}
	// Free other room resources here as needed
	free(room);
}
