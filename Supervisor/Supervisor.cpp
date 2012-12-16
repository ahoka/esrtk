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
   unsigned short ushort = 12344;
   unsigned char uchar = 213;
   unsigned int ulong = 123456789u;
   //   unsigned int ulonglong = 123456789123456789ull;
   console.printf("uint:\t%u\n", 0);
   console.printf("uint:\t%u\n", uint);
   console.printf("ushort:\t%hu\n", ushort);
   console.printf("uchar:\t%hhu\n", uchar);
   console.printf("ulong:\t%lu\n", ulong);
   //   console.printf("ulonglong: %llu\n", ulonglong);

   for (;;)
   {
      asm volatile("hlt");
   }

   //   KASSERT(1 == 1);
}
