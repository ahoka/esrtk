#include <string.h>

void *
memset(void *s, int c, size_t n)
{
   char* p = (char *)s;

   for (size_t i = 0; i < n; i++)
   {
      *p++ = (char )c;
   }

   return s;
}

void *
memcpy(void *dest, const void *src, size_t size)
{
   char* d = (char *)dest;
   char* s = (char *)src;

   for (size_t i = 0; i < size; i++)
   {
      *d++ = *s++;
   }

   return dest;
}
