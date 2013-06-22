#include "posix-tools.h"
#include <unistd.h>
#include <limits.h>
#include <string.h>

#define DEFAULT_OFFSET -10

const char * program_name;

struct { char f; long c, n; } options;

void usage() {
    pt_msg_usage("[-f] [-c number|-n number] [file]");
}

long file_offset(const char *str) {
    long n;

    switch (*str) {
        case '+': n = atol(str) - 1; break;
        case '-': n = atol(str); break;
        default:  n = -atol(str); break;
    }

    return n;
}

void seek_in_bytes(FILE *fp, long n) {
    if (fseek(fp, n, (n < 0) ? SEEK_END : SEEK_SET) != 0) {
        pt_die_fn("fseek");
    }
}

void seek_in_lines_backward(FILE *fp, long n) {
    char buf[LINE_MAX];

    long last_pos;
    long this_pos;

    long lines_read;
    long lines_over;

    n = labs(n);

    if (fseek(fp, -1, SEEK_END) != 0) {
        pt_die_fn("fseek");
    }

    while (lines_read < n) {
        last_pos = ftell(fp);
        this_pos = last_pos - (LINE_MAX * 10);

        if (this_pos < 0) this_pos = 0;

        if (fseek(fp, this_pos, SEEK_SET) != 0) {
            pt_die_fn("fseek");
        }

        while (fgets(buf, LINE_MAX, fp)) {
            ++lines_read;
        }
    }

    if (fseek(fp, this_pos, SEEK_SET) != 0) {
        pt_die_fn("fseek");
    }

    lines_over = lines_read - n;
    while (lines_over-- > 0) {
        fgets(buf, LINE_MAX, fp);
    }
}

void seek_in_lines_forward(FILE *fp, long n) {
    char buf[LINE_MAX];
    long lines;

    for (lines = 0; lines < n; lines++) {
        if (fgets(buf, LINE_MAX, fp) == NULL) break;
    }
}

void seek_in_lines(FILE *fp, long n) {
    if (n < 0) {
        seek_in_lines_backward(fp, n);
    } else {
        seek_in_lines_forward(fp, n);
    }
}

void tail(FILE *fp) {
    char buf[LINE_MAX];

    if (options.c) seek_in_bytes(fp, options.c);
    if (options.n) seek_in_lines(fp, options.n);

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

    while ((opt = getopt(argc, argv, "fc:n:")) != -1) {
        switch (opt) {
            case 'f': options.f = 1; break;
            case 'c': options.c = file_offset(optarg); break;
            case 'n': options.n = file_offset(optarg); break;
            default:  usage(); break;
        }
    }

    if (options.c && options.n) {
        usage();
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
