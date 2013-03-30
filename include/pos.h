#ifndef _POS_H
#define _POS_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void err(const char *err_func);
void die(const char *err_func);
void die_msg(const char *format, ...);

#endif /* _POS_H */
