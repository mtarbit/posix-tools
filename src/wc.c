#include "posix-tools.h"
#include <limits.h>
#include <locale.h>
#include <ctype.h>
#include <getopt.h>
#include <math.h>

const char *program_name;

struct counts { int l, w, m, c; };

struct counts options = {0, 0, 0, 0};

void usage() {
    pt_msg_usage("[-lwmc]... [file]...");
}

void print_counts(struct counts *widths, struct counts *counts, const char *label) {
    if (options.l) printf("%*d ", widths->l, counts->l);
    if (options.w) printf("%*d ", widths->w, counts->w);
    if (options.m) printf("%*d ", widths->m, counts->m);
    if (options.c) printf("%*d ", widths->c, counts->c);
    puts(label);
}

int number_of_digits(n) {
    return ceil(log10(n));
}

int main(int argc, char *argv[]) {
    int i, j;
    int len;
    int opt;

    int filec;
    char **filev;

    FILE *fp;
    char buf[LINE_MAX];
    char curr_chr;
    char last_chr;

    program_name = argv[0];

    while ((opt = getopt(argc, argv, "lwmc")) != -1) {
        switch (opt) {
            case 'l': options.l = 1; break;
            case 'w': options.w = 1; break;
            case 'm': options.m = 1; break;
            case 'c': options.c = 1; break;
            default:  usage();
        }
    }

    if (!options.l && !options.w && !options.m && !options.c) {
        options.l = 1; options.w = 1; options.c = 1;
    }

    filec = argc - optind;
    filev = argv + optind;

    setlocale(LC_ALL, "");

    struct counts counts[filec];
    struct counts totals = {0, 0, 0, 0};
    struct counts widths;

    for (i = 0; i < filec; i++) {
        if ((fp = fopen(filev[i], "r")) == NULL) {
            pt_die_fn("fopen");
        }

        counts[i].l = 0;
        counts[i].w = 0;
        counts[i].m = 0;
        counts[i].c = 0;

        while (fgets(buf, LINE_MAX, fp) != NULL) {
            for (j = 0; buf[j] && (len = mblen(&buf[j], LINE_MAX - j)) > 0; j += len) {
                counts[i].c += len;
                counts[i].m++;

                curr_chr = buf[j];

                if (curr_chr == '\n') {
                    counts[i].l++;
                }

                if (!isspace(curr_chr) && (!last_chr || isspace(last_chr))) {
                    counts[i].w++;
                }

                last_chr = curr_chr;
            }
        }

        totals.l += counts[i].l;
        totals.w += counts[i].w;
        totals.m += counts[i].m;
        totals.c += counts[i].c;
    }

    widths.l = number_of_digits(totals.l);
    widths.w = number_of_digits(totals.w);
    widths.m = number_of_digits(totals.m);
    widths.c = number_of_digits(totals.c);

    for (i = 0; i < filec; i++) {
        print_counts(&widths, &counts[i], filev[i]);
    }

    if (filec > 1) {
        print_counts(&widths, &totals, "total");
    }

    exit(EXIT_SUCCESS);
}
