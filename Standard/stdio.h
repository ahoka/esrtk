#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

int putchar(int);
int puts(const char *);

__attribute__((__format__ (__printf__, 1, 2))) int printf(const char* , ...);
int vprintf(const char* , va_list);

#endif
