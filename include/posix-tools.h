#ifndef _POSIX_TOOLS_H
#define _POSIX_TOOLS_H

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void pt_err(const char *fmt, ...);
void pt_err_fn(const char *fn_name);
void pt_msg_err(const char *fmt, ...);

void pt_die(const char *fmt, ...);
void pt_die_fn(const char *fn_name);
void pt_msg_die(const char *fmt, ...);

void pt_msg_usage(const char *fmt, ...);
void pt_msg_prompt(const char *fmt, ...);
char pt_msg_confirm(const char *fmt, ...);

int pt_scan_skip_hidden(const struct dirent *ent);
int pt_scan_skip_special(const struct dirent *ent);
int pt_scan_sort_alpha(const struct dirent **e1, const struct dirent **e2);

int pt_scandir(const char *path, struct dirent ***ents,
               int (*skip)(const struct dirent *ent),
               int (*sort)(const struct dirent **e1, const struct dirent **e2));

#endif /* _POSIX_TOOLS_H */
