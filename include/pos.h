#ifndef _POS_H
#define _POS_H

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int scan_skip_hidden(const struct dirent *ent);
int scan_skip_special(const struct dirent *ent);
int scan_sort_alpha(const struct dirent **e1, const struct dirent **e2);

char confirm();

void err(const char *err_func);
void die(const char *err_func);
void die_msg(const char *format, ...);

#endif /* _POS_H */
