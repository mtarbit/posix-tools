#include "pos.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>

int list_skip_hidden(const struct dirent *ent) {
    return *ent->d_name != '.';
}

int list_skip_special(const struct dirent *ent) {
    return strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0;
}

int list_sort_alpha(const struct dirent **e1, const struct dirent **e2) {
    return -1 * strcasecmp((*e1)->d_name, (*e2)->d_name);
}

void usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [-Aaf]... [FILE]...\n", program_name);
    exit(EXIT_FAILURE);
}

void list_file(const char *name) {
    puts(name);
}

void list_dir(const char *name, int (*skip)(const struct dirent *), int (*sort)(const struct dirent **, const struct dirent **)) {
    int n;
    struct dirent **ents;

    if ((n = scandir(name, &ents, skip, sort)) == -1) {
        err("scandir");
    } else {
        while (n--) {
            list_file(ents[n]->d_name);
            free(ents[n]);
        }
        free(ents);
    }
}

int main(int argc, char *argv[]) {
    int opt;

    int (*list_skip)(const struct dirent *);
    int (*list_sort)(const struct dirent **, const struct dirent **);

    int i;
    int filec;
    char **filev;
    struct stat statbuf;

    int A_option = 0,
        a_option = 0,
        f_option = 0;

    while ((opt = getopt(argc, argv, "Aaf")) != -1) {
        switch (opt) {
            case 'A': A_option = 1; break;
            case 'a': a_option = 1; break;
            case 'f': f_option = 1; break;
            default:  usage(argv[0]);
        }
    }

    if (a_option) {
        list_skip = 0;
    } else if (A_option) {
        list_skip = *list_skip_special;
    } else {
        list_skip = *list_skip_hidden;
    }

    if (f_option) {
        list_sort = 0;
    } else {
        list_sort = *list_sort_alpha;
    }

    filec = argc - optind;
    filev = &argv[optind];

    if (filec <= 0) {
        filec = 1; *filev = ".";
    }

    for (i = 0; i < filec; i++) {
        if (stat(filev[i], &statbuf) == -1) {
            err("stat"); continue;
        }

        if (i > 0) puts("");

        if (statbuf.st_mode & S_IFDIR) {
            if (filec > 1) printf("%s:\n", filev[i]);
            list_dir(filev[i], list_skip, list_sort);
        } else {
            list_file(filev[i]);
        }
    }

    exit(EXIT_SUCCESS);
}
