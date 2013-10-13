#include <string.h>

#if 0
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
#endif

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

#if 0
char *
strcpy(char *dest, const void *src)
{
   char* original = dest;

   while (*src != '\0')
   {
      *dest = *src++;
   }

   return original;
}

char *
strncpy(char *dest, const void *src, size_t size)
{
   char* original = dest;

   while (*src != '\0' && size--)
   {
      *dest = *src++;
   }

   while (size--)
   {
      *dest = '\0';
   }

   return original;
}
#endif

