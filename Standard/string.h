#ifndef STRING_H
#define STRING_H

#include <_size_t.h>

#ifdef __cplusplus
extern "C"
{
#endif

void *memcpy(void *, const void *, size_t);
void *memset(void *, int, size_t);

#ifdef __cplusplus
}
#endif

#endif
