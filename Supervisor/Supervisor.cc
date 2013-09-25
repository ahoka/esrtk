#include <Supervisor.hh>

#include <Platform.hh>
#include <Debug.hh>
#include <Power.hh>

#include <cstdio>

extern "C" void
supervisor_init()
{
   Supervisor::init();
}

void
Supervisor::run()
{
   printf("Supervisor started\n");

   Platform::init();

   Power::halt();
}
