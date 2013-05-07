#include "pos.h"
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

void remove_dir(const char *dir);

int f_option = 0,
    i_option = 0,
    r_option = 0;

char *program_name;

void usage() {
    die_msg("%s [-fiRr] file...\n", program_name);
}

char *join_path(const char *a, const char *b) {
    size_t la = strlen(a);
    size_t lb = strlen(b);
    size_t path_l = la + lb + 2;
    char *path = malloc(path_l);

    snprintf(path, path_l, "%s/%s", a, b);

    return path;
}

const char *file_description(struct stat *sb) {
    switch (sb->st_mode & S_IFMT) {
        case S_IFBLK:  return "block device";     break;
        case S_IFCHR:  return "character device"; break;
        case S_IFDIR:  return "directory";        break;
        case S_IFIFO:  return "FIFO/pipe";        break;
        case S_IFLNK:  return "symbolic link";    break;
        case S_IFREG:  return "regular file";     break;
        case S_IFSOCK: return "socket";           break;
        default:       return "unknown?";         break;
    }
}

int file_not_writable(const char *file) {
    return access(file, W_OK) == -1;
}

int input_is_terminal() {
    return isatty(fileno(stdin));
}

void remove_file(const char *file) {
    struct stat statbuf;

    if (lstat(file, &statbuf) == -1) {
        if (!f_option) {
            fprintf(stderr, "%s: cannot remove '%s': %s", program_name, file, strerror(errno));
        }
        return;
    }

    if (S_ISDIR(statbuf.st_mode)) {

        if (!r_option) {
            fprintf(stderr, "%s: cannot remove '%s': Is a directory\n", program_name, file);
            return;
        }

        if (!f_option && ((file_not_writable(file) && input_is_terminal()) || i_option)) {
            fprintf(stderr, "%s: descend into directory '%s'? ", program_name, file);
            if (confirm() != 'y') {
                return;
            }
        }

        remove_dir(file);

        if (i_option) {
            fprintf(stderr, "%s: remove directory '%s'? ", program_name, file);
            if (confirm() != 'y') {
                return;
            }
        }

        if (rmdir(file) == -1) {
            err("unlink");
        }

    } else {

        if (!f_option && ((file_not_writable(file) && input_is_terminal()) || i_option)) {
            fprintf(stderr, "%s: remove %s '%s'? ", program_name, file_description(&statbuf), file);
            if (confirm() != 'y') {
                return;
            }
        }

        if (unlink(file) == -1) {
            err("unlink");
        }

    }
}

void remove_dir(const char *dir) {
    int n;
    struct dirent **ents;
    char *path;

    if ((n = scandir(dir, &ents, scan_skip_special, scan_sort_alpha)) == -1) {
        err("scandir");
    } else {
        while (n--) {
            path = join_path(dir, ents[n]->d_name);

            remove_file(path);

            free(path);
            free(ents[n]);
        }
        free(ents);
    }
}

int main(int argc, char *argv[]) {
    int opt;

    int i;
    int filec;
    char **filev;

    program_name = argv[0];

    while ((opt = getopt(argc, argv, "fiRr")) != -1) {
        switch (opt) {
            case 'f': f_option = 1; break;
            case 'i': i_option = 1; break;
            case 'R':
            case 'r': r_option = 1; break;
            default:  usage();
        }
    }

    filec = argc - optind;
    filev = argv + optind;

    if (filec == 0) {
        usage();
    }

    for (i = 0; i < filec; i++) {
        remove_file(filev[i]);
    }

    exit(EXIT_SUCCESS);
}
