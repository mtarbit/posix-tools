#include "pos.h"
#include <string.h>

int main(int argc, char *argv[]) {
    int i;
    int n_option;

    --argc; ++argv;

    if (argc > 0 && strcmp(argv[0], "-n") == 0) {
        --argc; ++argv;
        n_option = 1;
    } else {
        n_option = 0;
    }

    for (i = 0; i < argc; i++) {
        if (i > 0) putchar(' ');
        printf("%s", argv[i]);
    }

    if (!n_option) putchar('\n');

    exit(EXIT_SUCCESS);
}
