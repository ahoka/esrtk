#include <X86/IoPort.hh>
#include <Power.hh>

#include <cstdio>

void
Power::halt()
{
   asm volatile("hlt");
}

void
Power::pause()
{
   asm volatile("pause");
}

void
Power::shutdown()
{
   printf("\nHalting CPU.\n");

   asm volatile("cli");

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
