#include "posix-tools.h"
#include <unistd.h>

const char *program_name;

int main(int argc, char *argv[]) {
    long path_max;
    size_t buf_size;

    char *buf;
    char *ptr;

    program_name = argv[0];

    path_max = pathconf(".", _PC_PATH_MAX);
    if (path_max == -1) {
        buf_size = 1024;
    } else {
        buf_size = path_max;
    }

    for (buf = ptr = NULL; ptr == NULL; buf_size *= 2) {
        if ((buf = realloc(buf, buf_size)) == NULL) {
            pt_die_fn("realloc");
        }

        if ((ptr = getcwd(buf, buf_size)) == NULL && errno != ERANGE) {
            pt_die_fn("getcwd");
        }
    }

    printf("%s\n", buf);

    free(buf);

    exit(EXIT_SUCCESS);
}
