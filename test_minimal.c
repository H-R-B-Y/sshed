#include <stdio.h>
#include <stdlib.h>
#include <notcurses/notcurses.h>

int main(void) {
    // Try the most minimal initialization possible
    struct notcurses_options opts = {
        .flags = NCOPTION_SUPPRESS_BANNERS | 
                 NCOPTION_NO_ALTERNATE_SCREEN | 
                 NCOPTION_NO_CLEAR_BITMAPS |
                 NCOPTION_INHIBIT_SETLOCALE |
                 NCOPTION_NO_QUIT_SIGHANDLERS,
        .loglevel = NCLOGLEVEL_ERROR,
    };
    
    printf("Starting minimal notcurses test...\n");
    fflush(stdout);
    
    struct notcurses* nc = notcurses_init(&opts, NULL);
    if (!nc) {
        printf("Failed to initialize notcurses\n");
        return 1;
    }
    
    printf("Success! Notcurses initialized.\n");
    
    // Clean shutdown
    notcurses_stop(nc);
    printf("Notcurses stopped cleanly.\n");
    
    return 0;
}
