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

   const char* s = "Hello World!";
   
   console.printf("Supervisor started\n\n");

   unsigned int uint = 123456789u;
   unsigned short ushort = 12345;
   unsigned char uchar = 213;
   unsigned int ulong = 123456789ul;

   int sint = -123456789;
   short sshort = -12345;
   char schar = -123;
   int slong = -123456789l;
   
   unsigned long long ulonglong = 123456789123456789ull;

   console.printf("uint:\t\t%u\n", 0);
   console.printf("uint:\t\t%u\n", uint);
   console.printf("ushort:\t\t%hu\n", ushort);
   console.printf("uchar:\t\t%hhu\n", uchar);
   console.printf("ulong:\t\t%lu\n\n", ulong);

   console.printf("sint:\t\t%d\n", 0);
   console.printf("sint:\t\t%d\n", sint);
   console.printf("sshort:\t\t%hd\n", sshort);
   console.printf("schar:\t\t%hhd\n", schar);
   console.printf("slong:\t\t%ld\n\n", slong);

   console.printf("uint:\t\t0x%x\n", 0);
   console.printf("uint:\t\t0x%x\n", uint);
   console.printf("ushort:\t\t0x%hx\n", ushort);
   console.printf("uchar:\t\t0x%hhx\n", uchar);
   console.printf("ulong:\t\t0x%lx\n", ulong);
#if 0
   console.printf("uint:\t\t0X%X\n", 0);
   console.printf("uint:\t\t0X%X\n", uint);
   console.printf("ushort:\t\t0X%hX\n", ushort);
   console.printf("uchar:\t\t0X%hhX\n", uchar);
   console.printf("ulong:\t\t0X%lX\n", ulong);
#endif

   console.printf("string: %s\n", s);

   console.printf("%c%c%c", 'a', '3', '\n');

   console.printf("uint:\t\t0%o\n", 0);
   console.printf("uint:\t\t0%o\n", uint);
   console.printf("ushort:\t\t0%ho\n", ushort);
   console.printf("uchar:\t\t0%hho\n", uchar);
   console.printf("ulong:\t\t0%lo\n", ulong);

   for (;;)
   {
      asm volatile("hlt");
   }

   //   KASSERT(1 == 1);
}
