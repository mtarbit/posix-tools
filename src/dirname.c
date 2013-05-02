#include "pos.h"
#include <libgen.h>

void usage(const char *program_name) {
    die_msg("Usage: %s NAME\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage(argv[0]);
    }

    puts(dirname(argv[1]));

    exit(EXIT_SUCCESS);
}
