#include "posix-tools.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define DEFAULT_OFFSET -10

const char * program_name;

struct Options { long long c, n; char f; } options;

void usage() {
    pt_msg_usage("[-f] [-c number|-n number] [file]");
}

long long file_offset(const char *str) {
    long long n;

    switch (*str) {
        case '+': n = atoll(str) - 1; break;
        case '-': n = atoll(str); break;
        default:  n = -atoll(str); break;
    }

    return n;
}

void tail(FILE *fp) {
    char buf[LINE_MAX];

    fseek(fp, options.c, (options.c < 0) ? SEEK_END : SEEK_SET);

    while (fgets(buf, LINE_MAX, fp)) {
        fputs(buf, stdout);
    }
}

int main(int argc, char *argv[]) {
    int opt;

    int filec;
    char **filev;

    FILE *fp;

    program_name = argv[0];

    while ((opt = getopt(argc, argv, "c:fn:")) != -1) {
        switch (opt) {
            case 'c': options.c = file_offset(optarg); break;
            case 'f': options.f = 1; break;
            case 'n': options.n = file_offset(optarg); break;
            default:  usage(); break;
        }
    }

    if (!options.c && !options.n) {
        options.n = DEFAULT_OFFSET;
    }

    filec = argc - optind;
    filev = argv + optind;

    if (filec == 1) {

        if ((fp = fopen(filev[0], "r")) == NULL) {
            pt_die_fn("fopen");
        }

        tail(fp);

        if (fclose(fp) != 0) {
            pt_die_fn("fclose");
        }

    } else if (filec == 0) {
        tail(stdin);
    } else {
        usage();
    }

    exit(EXIT_SUCCESS);
}
