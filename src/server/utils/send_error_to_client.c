
# include "server/server.h"


int	send_error_to_client(
	struct s_connection *conn,
	enum e_error_message_types error_type
)
{
	struct s_header_chunk	header;
	struct s_error_message	error_msg;

	header.msg_type = MTYPE_ERROR_MSG;
	header.content_length = sizeof(struct s_error_message);
	error_msg.error_type = error_type;
	return (send_to_connection_ref(
		get_server_instance(),
		conn,
		&header,
		&error_msg
	));
}
