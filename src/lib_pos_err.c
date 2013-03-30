#include "pos.h"
#include <string.h>
#include <stdarg.h>

void err(const char *err_func) {
    char *err_msg = "Error - Calling ";
    size_t buf_size = strlen(err_msg) + strlen(err_func) + 1;
    char buf[buf_size];

    snprintf(buf, buf_size, "%s%s", err_msg, err_func);

    if (errno) {
        perror(buf);
    } else {
        printf("%s.\n", buf);
    }
}

void die(const char *err_func) {
    err(err_func);
    exit(EXIT_FAILURE);
}

void die_msg(const char *format, ...) {
    va_list ap;

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

