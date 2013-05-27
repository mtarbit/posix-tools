#include "posix-tools.h"
#include <libgen.h>

const char *program_name;

void usage() {
    msg_usage("name");
}

int main(int argc, char *argv[]) {
    program_name = argv[0];

    if (argc != 2) {
        usage();
    }

    puts(dirname(argv[1]));

    exit(EXIT_SUCCESS);
}
