#include <StackTrace.hh>

#include <stdio.h>

typedef unsigned long reg_t;

void
StackTrace::printStackTrace()
{
   reg_t* ebp;

   asm volatile("movl %%ebp, %0" : "=g"(ebp));

   printf("Stack trace:\n");

   while (ebp != 0)
   {
      reg_t ip = *(ebp + 1);

      printf("  0x%x\n", ip);

      ebp = reinterpret_cast<reg_t*>(*ebp);
   }
}
