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

/*
Notes on options (for reference during development)
===================================================

Filtering
---------
x -A Write out all directory entries, including those whose names begin with a <period> ( '.' ) but excluding the entries dot and dot-dot (if they exist).
x -a Write out all directory entries, including those whose names begin with a <period> ( '.' ).

Sorting
-------
  -S Sort with the primary key being file size (in decreasing order) and the secondary key being filename in the collating sequence (in increasing order).
x -f List the entries in directory operands in the order they appear in the directory. The behavior for non-directory operands is unspecified. This option shall turn off -l, -t, -S, -s, and -r, and shall turn on -a.
  -r Reverse the order of the sort to get reverse collating sequence oldest first, or smallest file size first depending on the other options given.
  -t Sort with the primary key being time modified (most recently modified first) and the secondary key being filename in the collating sequence. For a symbolic link, the time used as the sort key is that of the symbolic link itself, unless ls is evaluating its file information to be that of the file referenced by the link (see the -H and -L options).

Column values
-------------
  -c Use time of last modification of the file status information (see <sys/stat.h> in the System Interfaces volume of POSIX.1-2008) instead of last modification of the file itself for sorting ( -t) or writing ( -l).
  -i For each file, write the file's file serial number (see stat() in the System Interfaces volume of POSIX.1-2008).
  -k Set the block size for the -s option and the per-directory block count written for the -l, -n, and -s  options (see the STDOUT section) to 1024 bytes.
  -n The same as -l, except that the owner's UID and GID numbers shall be written, rather than the associated character strings.
  -p Write a <slash> ( '/' ) after each filename if that file is a directory.
  -q Force each instance of non-printable filename characters and <tab> characters to be written as the <question-mark> ( '?' ) character. Implementations may provide this option by default if the output is to a terminal device.
  -s Indicate the total number of file system blocks consumed by each file displayed. If the -k option is also specified, the block size shall be 1024 bytes; otherwise, the block size is implementation-defined.
  -u Use time of last access (see <sys/stat.h>) instead of last modification of the file for sorting ( -t) or writing ( -l).

Symbolic links
--------------
  -H Evaluate the file information and file type for symbolic links specified on the command line to be those of the file referenced by the link, and not the link itself; however, ls shall write the name of the link itself and not the file referenced by the link.
x -L Evaluate the file information and file type for all symbolic links (whether named on the command line or encountered in a file hierarchy) to be those of the file referenced by the link, and not the link itself; however, ls shall write the name of the link itself and not the file referenced by the link. When -L is used with -l, write the contents of symbolic links in the long format (see the STDOUT section).

Output format
-------------
  -C Write multi-text-column output with entries sorted down the columns, according to the collating sequence. The number of text columns and the column separator characters are unspecified, but should be adapted to the nature of the output device.
x -l (The letter ell.) Do not follow symbolic links named as operands unless the -H or -L options are specified. Write out in long format (see the STDOUT section). When -l (ell) is specified, -1 (one) shall be assumed.
  -m Stream output format; list files across the page, separated by <comma> characters.
  -x The same as -C, except that the multi-text-column output is produced with entries sorted across, rather than down, the columns.
  -1 (The numeric digit one.) Force output to be one entry per line.

Miscellaneous
-------------
  -F Do not follow symbolic links named as operands unless the -H or -L options are specified. Write a <slash> ( '/' ) immediately after each pathname that is a directory, an <asterisk> ( '*' ) after each that is executable, a <vertical-line> ( '|' ) after each that is a FIFO, and an at-sign ( '@' ) after each that is a symbolic link. For other file types, other symbols may be written.
  -d Do not follow symbolic links named as operands unless the -H or -L options are specified. Do not treat directories differently than other types of files. The use of -d with -R produces unspecified results.
  -R Recursively list subdirectories encountered. When a symbolic link to a directory is encountered, the directory shall not be recursively listed unless the -L option is specified.
*/

int A_option = 0,
    L_option = 0,
    a_option = 0,
    f_option = 0,
    l_option = 0;

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
    fprintf(stderr, "Usage: %s [-ALafl]... [FILE]...\n", program_name);
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

void get_link_str(char *str, size_t str_max, const char *path_str) {
    str = realloc(str, str_max + 4);

    readlink(path_str, str, str_max);
    str[str_max] = '\0';

    memmove(str + 4, str, str_max);
    memmove(str, " -> ", 4);
}

void list_file_short(const char *dir_name, const char *name) {
    puts(name);
}

void list_file_long(const char *dir_name, const char *name) {
    size_t path_str_max = strlen(dir_name) + strlen(name) + 2;
    char path_str[path_str_max];

    char *link_str;

    size_t mode_str_max = 11;
    char mode_str[mode_str_max];

    struct stat sb;
    struct passwd *pb;
    struct group *gb;

    size_t time_str_max = 13;
    char time_str[time_str_max];

    snprintf(path_str, path_str_max, "%s/%s", dir_name, name);

    if (L_option) {
        if (stat(path_str, &sb) == -1) { err("stat"); return; }
    } else {
        if (lstat(path_str, &sb) == -1) { err("lstat"); return; }
    }

    if (L_option || !S_ISLNK(sb.st_mode)) {
        link_str = "";
    } else {
        link_str = malloc(sb.st_size + 1);
        get_link_str(link_str, sb.st_size + 1, path_str);
    }

    if ((pb = getpwuid(sb.st_uid)) == NULL) {
        err("getpwuid"); return;
    }

    if ((gb = getgrgid(sb.st_gid)) == NULL) {
        err("getgrgid"); return;
    }

    get_mode_str(mode_str, mode_str_max, sb.st_mode);
    get_time_str(time_str, time_str_max, sb.st_mtime);

    printf("%s %u %s %s %5u %s %s%s\n",
           mode_str,
           (unsigned int)sb.st_nlink,
           pb->pw_name,
           gb->gr_name,
           (unsigned int)sb.st_size,
           time_str,
           name,
           link_str);

    if (*link_str) free(link_str);
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

    while ((opt = getopt(argc, argv, "ALafl")) != -1) {
        switch (opt) {
            case 'A': A_option = 1; break;
            case 'L': L_option = 1; break;
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
    filev = argv + optind;

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
