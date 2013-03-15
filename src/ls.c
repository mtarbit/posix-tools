#include "pos.h"
#include <getopt.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>

int scandir_skip_hidden(const struct dirent *ent) {
    return *ent->d_name != '.';
}

int scandir_skip_special(const struct dirent *ent) {
    return strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0;
}

int scandir_sort_alpha(const struct dirent **e1, const struct dirent **e2) {
    return -1 * strcasecmp((*e1)->d_name, (*e2)->d_name);
}

void usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [-Aaf]... [FILE]...\n", program_name);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int opt;

    int n;
    struct dirent **namelist;

    int (*scandir_skip)(const struct dirent *);
    int (*scandir_sort)(const struct dirent **, const struct dirent **);

    int A_option, a_option, f_option;

    int i;
    int filec;
    char **filev;

    while ((opt = getopt(argc, argv, "Aaf")) != -1) {
        switch (opt) {
            case 'A': A_option = 1; break;
            case 'a': a_option = 1; break;
            case 'f': f_option = 1; break;
            default:  usage(argv[0]);
        }
    }

    if (a_option) {
        scandir_skip = 0;
    } else if (A_option) {
        scandir_skip = *scandir_skip_special;
    } else {
        scandir_skip = *scandir_skip_hidden;
    }

    if (f_option) {
        scandir_sort = 0;
    } else {
        scandir_sort = *scandir_sort_alpha;
    }

    filec = argc - optind;
    filev = &argv[optind];

    if (filec <= 0) {
        filec = 1; *filev = ".";
    }

    for (i = 0; i < filec; i++) {
        if ((n = scandir(filev[i], &namelist, scandir_skip, scandir_sort)) == -1) {
            die("scandir");
        }

        if (filec > 1) {
            if (i > 0) puts("");
            printf("%s:\n", filev[i]);
        }

        while (n--) {
            puts(namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }

    exit(EXIT_SUCCESS);
}
