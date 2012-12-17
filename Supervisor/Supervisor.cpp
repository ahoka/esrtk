/* 
 * Main class for the supervisor
 */

#include "Supervisor.h"
#include "Debug.h"
#include <VgaConsole.h>

Supervisor::Supervisor()
{
}

Supervisor::Supervisor(const Supervisor& /*orig*/)
{
}

Supervisor::~Supervisor()
{
}

void
Supervisor::run()
{
   VgaConsole console;
   
   console.write("Supervisor started\n\n");

   console.printf("Hello World\n");

   unsigned int uint = 123456789u;
   unsigned short ushort = 12345;
   unsigned char uchar = 213;
   unsigned int ulong = 123456789ul;
   //   unsigned int ulonglong = 123456789123456789ull;
   console.printf("uint:\t%u\n", 0);
   console.printf("uint:\t%u\n", uint);
   console.printf("ushort:\t%hu\n", ushort);
   console.printf("uchar:\t%hhu\n", uchar);
   console.printf("ulong:\t%lu\n\n", ulong);
   //   console.printf("ulonglong: %llu\n", ulonglong);

   console.printf("uint:\t0x%x\n", 0);
   console.printf("uint:\t0x%x\n", uint);
   console.printf("ushort:\t0x%hx\n", ushort);
   console.printf("uchar:\t0x%hhx\n", uchar);
   console.printf("ulong:\t0x%lx\n\n", ulong);

   console.printf("uint:\t0X%X\n", 0);
   console.printf("uint:\t0X%X\n", uint);
   console.printf("ushort:\t0X%hX\n", ushort);
   console.printf("uchar:\t0X%hhX\n", uchar);
   console.printf("ulong:\t0X%lX\n\n", ulong);

   for (;;)
   {
      asm volatile("hlt");
   }

   //   KASSERT(1 == 1);
}
