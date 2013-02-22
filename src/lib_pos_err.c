#include "pos.h"
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

