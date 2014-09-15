#ifndef STDLIB_H
#define STDLIB_H

#include <_cplusplus.h>
#include <_null.h>
#include <_size_t.h>

_CXX_BEGIN

void abort(void) __attribute__((noreturn));

void *malloc(size_t size);
void free(void *ptr);
void *realloc (void* ptr, size_t size);

int rand(void);
void srand(unsigned int);

int abs(int);

_CXX_END

#endif
