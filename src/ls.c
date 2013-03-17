#include "pos.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

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
    fprintf(stderr, "Usage: %s [-Aafl]... [FILE]...\n", program_name);
    exit(EXIT_FAILURE);
}

void get_mode_str(char *str, size_t str_max, mode_t mode) {
    char format_char;
    char owner_str[4];
    char group_str[4];
    char other_str[4];

    switch (mode & S_IFMT) {
        case S_IFBLK: format_char = 'b'; break;
        case S_IFCHR: format_char = 'c'; break;
        case S_IFDIR: format_char = 'd'; break;
        case S_IFIFO: format_char = 'p'; break;
        case S_IFLNK: format_char = 'l'; break;
        default:      format_char = '-'; break;
    }

    snprintf(owner_str, 4, "%c%c%c",
             mode & S_IRUSR ? 'r':'-',
             mode & S_IWUSR ? 'w':'-',
             mode & S_ISUID ? (mode & S_IXUSR ? 's':'S')
                            : (mode & S_IXUSR ? 'x':'-'));

    snprintf(group_str, 4, "%c%c%c",
             mode & S_IRGRP ? 'r':'-',
             mode & S_IWGRP ? 'w':'-',
             mode & S_ISGID ? (mode & S_IXGRP ? 's':'S')
                            : (mode & S_IXGRP ? 'x':'-'));

    snprintf(other_str, 4, "%c%c%c",
             mode & S_IROTH ? 'r':'-',
             mode & S_IWOTH ? 'w':'-',
             mode & S_IXOTH ? 'x':'-');

    snprintf(str, str_max, "%c%s%s%s", format_char, owner_str, group_str, other_str);
}

void get_time_str(char *str, size_t str_max, time_t t) {
    char *fmt;

    if (t > time(NULL) - 60 * 60 * 24 * 30 * 6) {
        fmt = "%b %e %H:%M";
    } else {
        fmt = "%b %e %_5Y";
    }

    strftime(str, str_max, fmt, localtime(&t));
}

void list_file_short(const char *dir_name, const char *name) {
    puts(name);
}

void list_file_long(const char *dir_name, const char *name) {
    size_t path_str_max = strlen(dir_name) + strlen(name) + 2;
    char path_str[path_str_max];

    size_t mode_str_max = 11;
    char mode_str[mode_str_max];

    struct stat sb;
    struct passwd *pb;
    struct group *gb;

    size_t time_str_max = 13;
    char time_str[time_str_max];

    snprintf(path_str, path_str_max, "%s/%s", dir_name, name);

    if (lstat(path_str, &sb) == -1) {
        err("lstat"); return;
    }

    if ((pb = getpwuid(sb.st_uid)) == NULL) {
        err("getpwuid"); return;
    }

    if ((gb = getgrgid(sb.st_gid)) == NULL) {
        err("getgrgid"); return;
    }

    get_mode_str(mode_str, mode_str_max, sb.st_mode);
    get_time_str(time_str, time_str_max, sb.st_mtime);

    printf("%s %u %s %s %5u %s %s\n",
           mode_str,
           (unsigned int)sb.st_nlink,
           pb->pw_name,
           gb->gr_name,
           (unsigned int)sb.st_size,
           time_str,
           name);
}

void list_dir(const char *dir_name,
              int (*skip)(const struct dirent *),
              int (*sort)(const struct dirent **, const struct dirent **),
              void (*list)(const char *, const char *)) {
    int n;
    struct dirent **ents;

    if ((n = scandir(dir_name, &ents, skip, sort)) == -1) {
        err("scandir");
    } else {
        while (n--) {
            list(dir_name, ents[n]->d_name);
            free(ents[n]);
        }
        free(ents);
    }
}

int main(int argc, char *argv[]) {
    int opt;

    int (*list_skip)(const struct dirent *);
    int (*list_sort)(const struct dirent **, const struct dirent **);
    void (*list_file)(const char *, const char *);

    int i;
    int filec;
    char **filev;
    struct stat statbuf;

    int A_option = 0,
        a_option = 0,
        f_option = 0,
        l_option = 0;

    while ((opt = getopt(argc, argv, "Aafl")) != -1) {
        switch (opt) {
            case 'A': A_option = 1; break;
            case 'a': a_option = 1; break;
            case 'f': f_option = 1; break;
            case 'l': l_option = 1; break;
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

    if (l_option) {
        list_file = *list_file_long;
    } else {
        list_file = *list_file_short;
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

        if (statbuf.st_mode & S_IFDIR) {
            if (filec > 1) printf("%s%s:\n", (i>0 ? "\n":""), filev[i]);
            list_dir(filev[i], list_skip, list_sort, list_file);
        } else {
            list_file(".", filev[i]);
        }
    }

    exit(EXIT_SUCCESS);
}
