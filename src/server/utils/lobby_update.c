#include "server/server.h"

struct s_lobby_update	*create_lobby_update(
	enum e_lobby_update_type update_type, t_u32 room, ...
)
{
	va_list					args;
	struct s_lobby_update	*update;

	update = ft_calloc(1, sizeof(struct s_lobby_update));
	if (!update)
		return (NULL);
	update->update_type = update_type;
	update->room_id = room;
	va_start(args, room);
	switch (update_type)
	{
		case LOBBY_PLAYER_JOINED:
			update->data.player_joined.player_id = (t_u8)va_arg(args, int);
			{
				char	*name_arg = va_arg(args, char *);
				ft_strlcpy(update->data.player_joined.player_name,
					name_arg, sizeof(update->data.player_joined.player_name));
			}
			break;
		case LOBBY_PLAYER_LEFT:
			update->data.player_left.player_id = (t_u8)va_arg(args, int);
			break;
		case LOBBY_SETTINGS_CHANGED:
			update->data.settings_changed.starting_hand_size = (t_u8)va_arg(args, int);
			break;
		case LOBBY_PLAYER_READY_STATUS_CHANGED:
			update->data.player_ready_status_changed.player_id = (t_u8)va_arg(args, int);
			update->data.player_ready_status_changed.is_ready = (t_u8)va_arg(args, int);
			break;
		case LOBBY_CLOSED:
			// No additional data
			break;
	}
	va_end(args);
	return (update);
}
