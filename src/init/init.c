
#include <errno.h>
#include <string.h>
#include "game.h"
/*
In init we need to make sure that the images are installed.
(future) We need to check if the server is running and start it if not. (figure this out later)
We need to load the cards into memory (potentially in future we should use some form of shared
memory for all clients as they all run on the same system)
*/

#define ERR(reason) {dprintf(STDERR_FILENO, "Failed to initialize client\nReason: %s\nerrno: %s\n", reason, strerror(errno));return (1);}

int	init_notcurses(
	struct notcurses **nc
)
{
	struct notcurses_options	opts = {
		.flags = NCOPTION_SUPPRESS_BANNERS | NCOPTION_NO_ALTERNATE_SCREEN,
		.loglevel = NCLOGLEVEL_SILENT,
		.termtype = "xterm-256color",
	};

	if (!nc)
		ERR("Input notcurses pointer is NULL");
	(*nc) = notcurses_init(&opts, NULL);
	if (!(*nc))
		ERR("Failed to init notcurses");
	return (0);
}

int init_hand(
	struct notcurses *nc,
	struct s_hand **hand
)
{
	unsigned int				width, height;
	struct ncplane	*stdplane = notcurses_stdplane(nc);

	if (!hand)
		ERR("Input hand double pointer is NULL");
	if (!stdplane)
		ERR("Standard plane is NULL (maybe notcurses not initialized?)");
	ncplane_dim_yx(stdplane, &height, &width);
	(*hand) = NULL;
	if (hand_create(hand, stdplane))
		ERR("Failed to create hand");
	return (0);
}

int	init_client(
	struct notcurses **nc,
	struct s_deck **deck,
	struct s_hand **hand
)
{
	if (!nc || !deck || !hand)
		ERR("One or more input pointers are NULL");
	if (init_notcurses(nc))
		ERR("Failed to initialize notcurses");
	if (init_cards(*nc) != 0)
		ERR("Failed to initialize cards");
	(*deck) = deck_create(true);
	if (!deck)
		ERR("Failed to create deck");
	for (int i = 0; i < 5; i++)
		deck_shuffle((*deck), (unsigned int)time(NULL));
	if (init_hand(*nc, hand))
		ERR("Failed to initialize hand");
	notcurses_refresh(*nc, NULL, NULL);
	return (0);
}
