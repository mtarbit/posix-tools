#include "pos.h"
#include <limits.h>
#include <locale.h>
#include <ctype.h>

// For -m flag see discussion of multi-byte / wide-char handling here:
// http://stackoverflow.com/questions/14083706/iterating-through-a-char-array-with-non-standard-chars/14083757#14083757

int main(int argc, char *argv[]) {
    int i, j;
    int len;

    int l_count,
        w_count,
        m_count,
        c_count;

    int l_total = 0,
        w_total = 0,
        m_total = 0,
        c_total = 0;

    FILE *fp;
    char buf[LINE_MAX];
    char curr_chr;
    char last_chr;

    setlocale(LC_ALL, "");

    for (i = 1; i < argc; i++) {
        if ((fp = fopen(argv[i], "r")) == NULL) {
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

                // printf("%d: '%.*s', %d\n", m_count, len, &buf[j], len);

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

        printf("%d %d %d %s\n", l_count, w_count, c_count, argv[i]);
    }

    if (argc > 2) {
        printf("%d %d %d %s\n", l_total, w_total, c_total, "total");
    }

    exit(EXIT_SUCCESS);
}
