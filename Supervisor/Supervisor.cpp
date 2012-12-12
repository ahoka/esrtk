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
   for (int i = 0; i < 30; i++)
   {
      console.write("Just a test string...\n");
   }
   console.write("Supervisor...");

   for (;;)
   {
      asm volatile("pause");
   }

   //   KASSERT(1 == 1);
}
