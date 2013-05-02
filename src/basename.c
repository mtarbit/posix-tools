#include "pos.h"
#include <libgen.h>
#include <string.h>

void usage(const char *program_name) {
    die_msg("Usage: %s NAME [SUFFIX]\n", program_name);
}

int main(int argc, char *argv[]) {
    char *name;
    char *suff;

    size_t name_l;
    size_t suff_l;
    size_t suff_i;

    if (argc < 2 || argc > 3) {
        usage(argv[0]);
    }

    name = basename(argv[1]);

    if (argc == 3) {
        suff = argv[2];

        name_l = strlen(name);
        suff_l = strlen(suff);
        suff_i = name_l - suff_l;

        if (suff_i && strncmp((name + suff_i), suff, suff_l) == 0) {
            name[suff_i] = '\0';
        }
    }

    puts(name);

    exit(EXIT_SUCCESS);
}
