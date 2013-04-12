#include "pos.h"
#include <limits.h>
#include <locale.h>
#include <ctype.h>
#include <getopt.h>

// For -m flag see discussion of multi-byte / wide-char handling here:
// http://stackoverflow.com/questions/14083706/iterating-through-a-char-array-with-non-standard-chars/14083757#14083757

int l_option = 0,
    w_option = 0,
    m_option = 0,
    c_option = 0;

void usage(const char *program_name) {
    die_msg("Usage: %s [-lwmc]... [FILE]...", program_name);
}

void print_counts(int l, int w, int m, int c, const char *label) {
    if (l_option || w_option || m_option || c_option) {
        if (l_option) printf("%d ", l);
        if (w_option) printf("%d ", w);
        if (m_option) printf("%d ", m);
        if (c_option) printf("%d ", c);
        puts(label);
    } else {
        printf("%d %d %d %s\n", l, w, c, label);
    }
}

int main(int argc, char *argv[]) {
    int i, j;
    int len;
    int opt;

    int l_count,
        w_count,
        m_count,
        c_count;

    int l_total = 0,
        w_total = 0,
        m_total = 0,
        c_total = 0;

    int filec;
    char **filev;

    FILE *fp;
    char buf[LINE_MAX];
    char curr_chr;
    char last_chr;

    while ((opt = getopt(argc, argv, "lwmc")) != -1) {
        switch (opt) {
            case 'l': l_option = 1; break;
            case 'w': w_option = 1; break;
            case 'm': m_option = 1; break;
            case 'c': c_option = 1; break;
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

        l_count = 0;
        w_count = 0;
        m_count = 0;
        c_count = 0;

        while (fgets(buf, LINE_MAX, fp) != NULL) {
            for (j = 0; buf[j] && (len = mblen(&buf[j], LINE_MAX - j)) > 0; j += len) {
                c_count += len;
                m_count++;

                curr_chr = buf[j];

                if (curr_chr == '\n') {
                    l_count++;
                }

                if (!isspace(curr_chr) && (!last_chr || isspace(last_chr))) {
                    w_count++;
                }

                last_chr = curr_chr;
            }
        }

        l_total += l_count;
        w_total += w_count;
        m_total += m_count;
        c_total += c_count;

        print_counts(l_count, w_count, m_count, c_count, filev[i]);
    }

    if (filec > 2) {
        print_counts(l_total, w_total, m_total, c_total, "total");
    }

    exit(EXIT_SUCCESS);
}
