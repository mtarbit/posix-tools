#ifndef _POS_H
#define _POS_H

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void err(const char *fmt, ...);
void err_fn(const char *fn_name);
void msg_err(const char *fmt, ...);

void die(const char *fmt, ...);
void die_fn(const char *fn_name);
void msg_die(const char *fmt, ...);

void msg_usage(const char *fmt, ...);
void msg_prompt(const char *fmt, ...);
char msg_confirm(const char *fmt, ...);

int scan_skip_hidden(const struct dirent *ent);
int scan_skip_special(const struct dirent *ent);
int scan_sort_alpha(const struct dirent **e1, const struct dirent **e2);

int pt_scandir(const char *path, struct dirent ***ents,
               int (*skip)(const struct dirent *ent),
               int (*sort)(const struct dirent **e1, const struct dirent **e2));

#endif /* _POS_H */
