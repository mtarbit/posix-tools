#include "posix-tools.h"
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

const char *program_name;

void remove_dir(const char *dir);

int f_option = 0,
    i_option = 0,
    r_option = 0;

void usage() {
    msg_usage("[-fiRr] file...");
}

char *join_path(const char *a, const char *b) {
    size_t path_len = strlen(a) + strlen(b) + 2;
    char *path = malloc(path_len);

    snprintf(path, path_len, "%s/%s", a, b);

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
            err("cannot remove '%s'", file);
        }
        return;
    }

    if (S_ISDIR(statbuf.st_mode)) {

        if (!r_option) {
            msg_err("cannot remove '%s': Is a directory", file);
            return;
        }

        if (!f_option && ((file_not_writable(file) && input_is_terminal()) || i_option)) {
            if (msg_confirm("descend into directory '%s'?", file) != 'y') {
                return;
            }
        }

        remove_dir(file);

        if (i_option) {
            if (msg_confirm("remove directory '%s'?", file) != 'y') {
                return;
            }
        }

        if (rmdir(file) == -1) {
            err_fn("unlink");
        }

    } else {

        if (!f_option && ((file_not_writable(file) && input_is_terminal()) || i_option)) {
            if (msg_confirm("remove %s '%s'?", file_description(&statbuf), file) != 'y') {
                return;
            }
        }

        if (unlink(file) == -1) {
            err_fn("unlink");
        }

    }
}

void remove_dir(const char *dir) {
    int n;
    struct dirent **ents;
    char *path;

    if ((n = pt_scandir(dir, &ents, scan_skip_special, scan_sort_alpha)) == -1) {
        err_fn("scandir");
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
