#include "pos.h"
#include <string.h>
#include <stdarg.h>
#include <limits.h>

int scan_skip_hidden(const struct dirent *ent) {
    return *ent->d_name != '.';
}

int scan_skip_special(const struct dirent *ent) {
    return strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0;
}

int scan_sort_alpha(const struct dirent **e1, const struct dirent **e2) {
    return -1 * strcasecmp((*e1)->d_name, (*e2)->d_name);
}

char confirm() {
    char buf[LINE_MAX];

    if ((fgets(buf, LINE_MAX, stdin)) == NULL) {
        die("fgets");
    }

    if (strncasecmp(buf, "y", 1) == 0) {
        return 'y';
    } else {
        return 'n';
    }
}

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

