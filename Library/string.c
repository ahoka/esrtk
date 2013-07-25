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
