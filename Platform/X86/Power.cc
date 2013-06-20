#include <Power.hh>
#include <IoPort.hh>
#include <stdio.h>

void
Power::halt()
{
   printf("\n\Halting CPU.\n");

   for (;;)
   {
      asm volatile("hlt");
   }
}

void
Power::reboot()
{
   printf("Rebooting...\n");

   for (int i = 0; i < 64; i++)
   {
      // reset through keyboard controller
      outb(0x64, 0xfe);
   }

   printf("Can't reboot CPU, halting instead!\n");

   for (;;)
   {
      asm volatile("hlt");
   }
}
