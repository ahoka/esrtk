#ifndef STDDEF_H
#define STDDEF_H

#ifdef __i386__

typedef int ptrdiff_t;
typedef unsigned int size_t;

#ifndef __cplusplus
typedef unsigned short wchar_t;
#  define NULL ((void *)0)
#else
#  define NULL 0
#endif

#define offsetof(s, m) ((size_t )&(((s *)NULL)->m))

#else
#  error Unsupported architecture!
#endif

#endif
