#include "posix-tools.h"
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>

extern const char *program_name;

void pt_err(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s: %s\n", program_name, buf, strerror(errno));
    va_end(ap);
}

void pt_err_fn(const char *fn_name) {
    pt_err("calling %s", fn_name);
}

void pt_msg_err(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s\n", program_name, buf);
    va_end(ap);
}

void pt_die(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    pt_err(fmt, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void pt_die_fn(const char *fn_name) {
    pt_err_fn(fn_name);
    exit(EXIT_FAILURE);
}

void pt_msg_die(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    pt_msg_err(fmt, ap);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void pt_msg_usage(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "Usage: %s %s\n", program_name, buf);
    va_end(ap);

    exit(EXIT_FAILURE);
}

void pt_msg_prompt(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s ", program_name, buf);
    va_end(ap);
}

char pt_msg_confirm(const char *fmt, ...) {
    va_list ap;
    char buf[LINE_MAX];

    va_start(ap, fmt);
    vsnprintf(buf, LINE_MAX, fmt, ap);
    fprintf(stderr, "%s: %s ", program_name, buf);
    va_end(ap);

    if (fgets(buf, LINE_MAX, stdin) == NULL) {
        pt_die_fn("fgets");
    }

    if (strncasecmp(buf, "y", 1) == 0) {
        return 'y';
    } else {
        return 'n';
    }
}

int pt_scan_skip_hidden(const struct dirent *ent) {
    return *ent->d_name != '.';
}

int pt_scan_skip_special(const struct dirent *ent) {
    return strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0;
}

int pt_scan_sort_alpha(const struct dirent **e1, const struct dirent **e2) {
    return -1 * strcasecmp((*e1)->d_name, (*e2)->d_name);
}

int pt_scandir(const char *path, struct dirent ***ents,
               int (*skip)(const struct dirent *ent),
               int (*sort)(const struct dirent **e1, const struct dirent **e2)) {
#if defined(_POSIX_VERSION) && (_POSIX_VERSION >= 200809L)
    return scandir(path, ents, skip, sort);
#else
    return scandir(path, ents, (int (*)(struct dirent *))skip, (int (*)(const void *, const void *))sort);
#endif
}


