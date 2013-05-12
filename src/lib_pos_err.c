#include "pos.h"
#include <string.h>
#include <stdarg.h>
#include <limits.h>

extern const char *program_name;

void err(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s: %s\n", program_name, buf, strerror(errno));
    va_end(ap);
}

void err_fn(const char *fn_name) {
    err("calling %s", fn_name);
}

void msg_err(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s\n", program_name, buf);
    va_end(ap);
}

void die(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    err(fmt, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void die_fn(const char *fn_name) {
    err_fn(fn_name);
    exit(EXIT_FAILURE);
}

void msg_die(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    msg_err(fmt, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void msg_usage(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "Usage: %s %s\n", program_name, buf);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void msg_prompt(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s ", program_name, buf);
    va_end(ap);
}

char msg_confirm(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s ", program_name, buf);
    va_end(ap);

    if (fgets(buf, LINE_MAX, stdin) == NULL) {
        die_fn("fgets");
    }

    if (strncasecmp(buf, "y", 1) == 0) {
        return 'y';
    } else {
        return 'n';
    }
}

int scan_skip_hidden(const struct dirent *ent) {
    return *ent->d_name != '.';
}

int scan_skip_special(const struct dirent *ent) {
    return strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0;
}

int scan_sort_alpha(const struct dirent **e1, const struct dirent **e2) {
    return -1 * strcasecmp((*e1)->d_name, (*e2)->d_name);
}
