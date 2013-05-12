#include "pos.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    long path_max;
    size_t buf_size;

    char *buf;
    char *ptr;

    path_max = pathconf(".", _PC_PATH_MAX);
    if (path_max == -1) {
        buf_size = 1024;
    } else {
        buf_size = path_max;
    }

    for (buf = ptr = NULL; ptr == NULL; buf_size *= 2) {
        if ((buf = realloc(buf, buf_size)) == NULL) {
            die_fn("realloc");
        }

        if ((ptr = getcwd(buf, buf_size)) == NULL && errno != ERANGE) {
            die_fn("getcwd");
        }
    }

    printf("%s\n", buf);

    free(buf);

    exit(EXIT_SUCCESS);
}
