#include <Power.hh>
#include <stdio.h>

void
Power::halt()
{
   printf("Will now halt.");

   for (;;)
   {
      asm volatile("hlt");
   }
}
