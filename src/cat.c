#include "pos.h"
#include <string.h>
#include <limits.h>

void putf(FILE *stream) {
    char buf[LINE_MAX];

    while (fgets(buf, LINE_MAX, stream)) {
        fputs(buf, stdout);
    }
}

int main(int argc, char *argv[]) {
    int i;
    FILE *fp;

    if (argc == 1) {

        putf(stdin);

    } else {

        for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-") == 0) {

                putf(stdin);

            } else {

                if ((fp = fopen(argv[i], "r")) == NULL) {
                    die("fopen");
                }

                putf(fp);

                if (fclose(fp) != 0) {
                    die("fclose");
                }

            }
        }

    }

    exit(EXIT_SUCCESS);
}
