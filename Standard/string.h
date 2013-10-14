#ifndef STRING_H
#define STRING_H

#include <_cplusplus.h>
#include <_size_t.h>

//ifdef _BSD_SOURCE
#include <strings.h>
//#endif

_CXX_BEGIN

void    *memchr(const void *, int, size_t);
int      memcmp(const void *, const void *, size_t);
void    *memcpy(void * restrict, const void * restrict, size_t);
void    *memmove(void *, const void *, size_t);
void    *memset(void *, int, size_t);
char    *strcat(char * restrict, const char * restrict);
char    *strchr(const char *, int);
int      strcmp(const char *, const char *);
int      strcoll(const char *, const char *);
char    *strcpy(char * restrict, const char * restrict);
size_t   strcspn(const char *, const char *);
char    *strerror(int);
size_t   strlen(const char *);
char    *strncat(char * restrict, const char * restrict, size_t);
int      strncmp(const char *, const char *, size_t);
char    *strncpy(char * restrict, const char * restrict, size_t);
char    *strpbrk(const char *, const char *);
char    *strrchr(const char *, int);
size_t   strspn(const char *, const char *);
char    *strstr(const char *, const char *);
char    *strtok(char * restrict, const char * restrict);

char    *stpcpy(char * restrict, const char * restrict);
char    *stpncpy(char * restrict, const char * restrict, size_t);
size_t  strnlen(const char *, size_t);
/* void *memcpy(void *dest, const void *src, size_t size); */
/* void *memset(void *dest, int ch, size_t size); */
/* int memcmp(const void *, const void *, size_t); */

//#ifdef _BSD_SOURCE
void    bzero(void *s, size_t n);
int     strcasecmp(const char *s1, const char *s2);
int     strncasecmp(const char *s1, const char *s2, size_t n);
size_t  strlcpy(char *dst, const char *src, size_t size);
size_t  strlcat(char *dst, const char *src, size_t siz);
char    *strsep(char **, const char *);
//#endif


_CXX_END

#endif
