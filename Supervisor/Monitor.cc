#include <Supervisor/Monitor.hh>
#include <cstdio>

Monitor::Monitor()
{
}

Monitor::~Monitor()
{
}

void
Monitor::enter()
{
   char buffer[128];
   while (gets_s(buffer, sizeof(buffer)))
   {
      puts(buffer);
   }
}
