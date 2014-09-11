#ifndef STDLIB_H
#define STDLIB_H

#include <_cplusplus.h>
#include <_null.h>
#include <_size_t.h>

_CXX_BEGIN

void abort(void) __attribute__((noreturn));
void *malloc(size_t size);
void free(void *ptr);

int rand(void);

_CXX_END

#endif
