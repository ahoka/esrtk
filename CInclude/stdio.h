#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>
#include <_size_t.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define EOF (-1)

int putchar(int);
int puts(const char *);

__attribute__((__format__ (__printf__, 1, 2))) int printf(const char* , ...);
int vprintf(const char* , va_list);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);

#ifdef __cplusplus
}
#endif

#endif
