#include <StackTrace.hh>

#include <cstdio>

typedef unsigned long reg_t;

void
StackTrace::printStackTrace(void* data)
{
   reg_t* ebp = reinterpret_cast<reg_t*>(data);

   printf("Stack trace:\n");

   while (ebp != 0)
   {
      reg_t ip = *(ebp + 1);

      printf("  0x%lx\n", ip);

      if (ip == 0)
      {
         break;
      }

      ebp = reinterpret_cast<reg_t*>(*ebp);
   }
}

void
StackTrace::printStackTrace()
{
   reg_t* ebp;

   asm volatile("movl %%ebp, %0" : "=g"(ebp));

   printStackTrace(ebp);
}
