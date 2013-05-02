#include "pos.h"
#include <libgen.h>
#include <string.h>

void usage(const char *program_name) {
    die_msg("Usage: %s NAME [SUFFIX]\n", program_name);
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

    if (argc < 2 || argc > 3) {
        usage(argv[0]);
    }

    name = basename(argv[1]);

    if (argc == 3) {
        name = strip_suffix(name, argv[2]);
    }

    puts(name);

    exit(EXIT_SUCCESS);
}
