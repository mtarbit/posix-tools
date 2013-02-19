#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void die(const char *err_func) {
    char *err_msg = "Error - Calling ";
    size_t buf_size = strlen(err_msg) + strlen(err_func) + 1;
    char buf[buf_size];

    snprintf(buf, buf_size, "%s%s", err_msg, err_func);

    if (errno) {
        perror(buf);
    } else {
        printf("%s.\n", buf);
    }

    exit(EXIT_FAILURE); 
}

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
            die("realloc");
        }

        if ((ptr = getcwd(buf, buf_size)) == NULL && errno != ERANGE) {
            die("getcwd");
        }
    }

    printf("%s\n", buf);

    free(buf);

    exit(EXIT_SUCCESS);
}
