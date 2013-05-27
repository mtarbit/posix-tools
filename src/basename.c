#include "posix-tools.h"
#include <libgen.h>
#include <string.h>

const char *program_name;

void usage() {
    msg_usage("name [suffix]");
}

char * strip_suffix(char *name, const char *suff) {
    size_t suff_l = strlen(suff);
    size_t suff_i = strlen(name) - suff_l;

    if (suff_i > 0 && strncmp((name + suff_i), suff, suff_l) == 0) {
        name[suff_i] = '\0';
    }

    return name;
}

int main(int argc, char *argv[]) {
    char *name;

    program_name = argv[0];

    if (argc < 2 || argc > 3) {
        usage();
    }

    name = basename(argv[1]);

    if (argc == 3) {
        name = strip_suffix(name, argv[2]);
    }

    puts(name);

    exit(EXIT_SUCCESS);
}
