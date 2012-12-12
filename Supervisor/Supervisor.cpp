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
   
   console.write("Supervisor started\n");
   console.write("Test\nTest\nTest\n\nT e s t\n");
   console.write("Just a test string...");

   for (;;)
   {
      asm volatile("pause");
   }

   //   KASSERT(1 == 1);
}
