#include "posix-tools.h"
#include <string.h>
#include <limits.h>

const char *program_name;

void putf(FILE *stream) {
    char buf[LINE_MAX];

    while (fgets(buf, LINE_MAX, stream)) {
        fputs(buf, stdout);
    }
}

int main(int argc, char *argv[]) {
    int i;
    FILE *fp;

    program_name = argv[0];

    if (argc == 1) {

        putf(stdin);

    } else {

        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0) {

                putf(stdin);

            } else {

                if ((fp = fopen(argv[i], "r")) == NULL) {
                    die_fn("fopen");
                }

                putf(fp);

                if (fclose(fp) != 0) {
                    die_fn("fclose");
                }

            }
        }

    }

    exit(EXIT_SUCCESS);
}
