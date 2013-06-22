#include "posix-tools.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define DEFAULT_OFFSET 10

const char * program_name;

void usage() {
    pt_msg_usage("[-n number] [file...]");
}

long file_offset(const char *str) {
    int n = atoi(str);
    if (n < 1) {
        pt_msg_die("The argument to option -n must be a positive number.");
    }
    return n;
}

void head(FILE *fp, int n) {
    int i;
    char buf[LINE_MAX];

    for (i = 0; i < n; i++) {
        if (fgets(buf, LINE_MAX, fp) == NULL) break;
        fputs(buf, stdout);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int option_n = -1;

    int filec;
    char **filev;
    int i;

    FILE *fp;

    program_name = argv[0];

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
            case 'n': option_n = file_offset(optarg); break;
            default:  usage(); break;
        }
    }

    if (option_n < 0) {
        option_n = DEFAULT_OFFSET;
    }

    filec = argc - optind;
    filev = argv + optind;

    if (filec == 0) {

        head(stdin, option_n);

    } else {

        for (i = 0; i < filec; i++) {
            if (i) puts("");

            if (strcmp(filev[i], "-") == 0) {

                if (filec > 1) {
                    printf("==> standard input <==\n");
                }

                head(stdin, option_n);

            } else {

                if (filec > 1) {
                    printf("==> %s <==\n", filev[i]);
                }

                if ((fp = fopen(filev[i], "r")) == NULL) {
                    pt_die_fn("fopen");
                }

                head(fp, option_n);

                if (fclose(fp) != 0) {
                    pt_die_fn("fclose");
                }

            }
        }

    }

    exit(EXIT_SUCCESS);
}
