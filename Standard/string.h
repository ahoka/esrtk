#ifndef STRING_H
#define STRING_H

#include <_size_t.h>

#ifdef __cplusplus
extern "C"
{
#endif

void *memcpy(void *, const void *, size_t);
void *memset(void *, int, size_t);

char* strcpy(char *dest, const char* src);
char* strncpy(char *dest, const char* src, size_t size);

size_t strlcpy(char *dst, const char *src, size_t size);

size_t strlen(const char *str);
int strcmp(const char *s1, const char *s2);

size_t strlcat(char *dst, const char *src, size_t siz);
char *strcat(char *s, const char *append);


#ifdef __cplusplus
}
#endif

#endif
