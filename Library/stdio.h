#ifndef _STDIO_H_
#define _STDIO_H_

#include <stdarg.h>

extern "C" {

int printf(const char* , ...);
int vprintf(const char* , va_list);

}

#endif
