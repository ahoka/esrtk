#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/systm.h>

int
copyinstr(const void * __restrict udaddr, void * __restrict kaddr,
          size_t len, size_t * __restrict lencopied)
{
   memcpy(kaddr, udaddr, len);
   if (lencopied)
   {
      *lencopied = strlen((const char*)udaddr);
   }
   
   return 0;
}

int
copyin(const void * __restrict udaddr, void * __restrict kaddr, size_t len)
{
   memcpy(kaddr, udaddr, len);
   
   return 0;
}

int
copyout(const void * __restrict kaddr, void * __restrict udaddr, size_t len)
{
   memcpy(udaddr, kaddr, len);
   
   return 0;
}
