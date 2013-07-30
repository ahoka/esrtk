#ifndef STDDEF_H
#define STDDEF_H

#include <_null.h>
#include <_size_t.h>

#ifdef __i386__

typedef int ptrdiff_t;

#ifndef __cplusplus
typedef unsigned short wchar_t;
#endif

#define offsetof(s, m) ((size_t )&(((s *)NULL)->m))

#else
#  error Unsupported architecture!
#endif

#endif
