
#include "test.h"
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	struct notcurses_options opts = {
		.flags = NCOPTION_SUPPRESS_BANNERS,
	};

	struct notcurses *nc = notcurses_init(&opts, NULL);
	if (!nc)
	{
		printf("Failed to init notcurses\n");
		return 1;
	}

	struct ncvisual *ncv = ncvisual_from_file("Joker Black.png");
	if (!ncv) {
		notcurses_stop(nc);
		printf("Failed open the file\n");
		return 1;
	}

	

	// Render the visual centered on the screen
	struct ncvisual_options vopts = {
		.n = notcurses_stdplane(nc),             // create a new plane
		.scaling = NCSCALE_NONE, // scale to fit screen
		.blitter = NCBLIT_PIXEL,
		.y = 0,
		.x = 0,
		.flags = NCVISUAL_OPTION_CHILDPLANE,
	};

	struct ncplane *plane = ncvisual_blit(nc, ncv, &vopts);
	if (!plane) {
		ncvisual_destroy(ncv);
		notcurses_stop(nc);
		printf("Failed to blit image\n");
		return 1;
	}

	// if (ncplane_putstr_yx(plane, 2, 2, "This is a test") < 1)
	// {
	// 	ncvisual_destroy(ncv);
	// 	notcurses_stop(nc);
	// 	printf("Failed to write text");
	// 	return (1);
	// }
	unsigned int rows, cols;

	notcurses_term_dim_yx(nc, &rows, &cols);
	char *str = calloc(10000, sizeof(char));
	sprintf(str, "size is %u, %u", rows, cols);
	ncplane_putstr_yx(notcurses_stdplane(nc), 0, 0, str);
	free(str);

	notcurses_render(nc);
	sleep(3); // keep it visible for a few seconds

	ncvisual_destroy(ncv);
	notcurses_stop(nc);
	return 0;
}