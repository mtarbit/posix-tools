#include "pos.h"
#include <limits.h>
#include <locale.h>
#include <ctype.h>
#include <getopt.h>

struct counts { int l, w, m, c; };

struct counts options = {0, 0, 0, 0};

void usage(const char *program_name) {
    die_msg("Usage: %s [-lwmc]... [FILE]...", program_name);
}

void print_counts(struct counts *counts, const char *label) {
    if (options.l || options.w || options.m || options.c) {
        if (options.l) printf("%d ", counts->l);
        if (options.w) printf("%d ", counts->w);
        if (options.m) printf("%d ", counts->m);
        if (options.c) printf("%d ", counts->c);
        puts(label);
    } else {
        printf("%d %d %d %s\n", counts->l, counts->w, counts->c, label);
    }
}

int main(int argc, char *argv[]) {
    int i, j;
    int len;
    int opt;

    struct counts counts;
    struct counts totals = {0, 0, 0, 0};

    int filec;
    char **filev;

    FILE *fp;
    char buf[LINE_MAX];
    char curr_chr;
    char last_chr;

    while ((opt = getopt(argc, argv, "lwmc")) != -1) {
        switch (opt) {
            case 'l': options.l = 1; break;
            case 'w': options.w = 1; break;
            case 'm': options.m = 1; break;
            case 'c': options.c = 1; break;
            default:  usage(argv[0]);
        }
    }

    filec = argc - optind;
    filev = &argv[optind];

    setlocale(LC_ALL, "");

    for (i = 0; i < filec; i++) {
        if ((fp = fopen(filev[i], "r")) == NULL) {
            die("fopen");
        }

        counts.l = 0;
        counts.w = 0;
        counts.m = 0;
        counts.c = 0;

        while (fgets(buf, LINE_MAX, fp) != NULL) {
            for (j = 0; buf[j] && (len = mblen(&buf[j], LINE_MAX - j)) > 0; j += len) {
                counts.c += len;
                counts.m++;

                curr_chr = buf[j];

                if (curr_chr == '\n') {
                    counts.l++;
                }

                if (!isspace(curr_chr) && (!last_chr || isspace(last_chr))) {
                    counts.w++;
                }

                last_chr = curr_chr;
            }
        }

        totals.l += counts.l;
        totals.w += counts.w;
        totals.m += counts.m;
        totals.c += counts.c;

        print_counts(&counts, filev[i]);
    }

    if (filec > 1) {
        print_counts(&totals, "total");
    }

    exit(EXIT_SUCCESS);
}
