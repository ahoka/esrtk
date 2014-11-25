#include <Supervisor/Monitor.hh>

#include <Time.hh>
#include <Power.hh>
#include <MemoryManager.hh>
#include <Kernel/Thread.hh>

#include <string>
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
      std::string command = buffer;

      puts(buffer);
#if 0
      if (!strcmp()
         printf("Interrupt statistics:\n");
         Interrupt::printStatistics();
         break;
      case Scancodes::F2:
      {
         uint64_t uptime = Time::getUptime();
         printf("Uptime is: %lu.%lu\n",
                (unsigned long )uptime / 1000,
                (unsigned long )(uptime % 1000));
         break;
      }
      case Scancodes::F3:
         putchar('\n');
         MemoryManager::get().printStatistics();
         break;
      case Scancodes::F4:
         putchar('\n');
         Kernel::Thread::printAll();
         break;
      case Scancodes::F9:
         Power::reboot();
//break;
      case Scancodes::F8:
         printf("Simulating a dead-lock!\n");
         for (;;)
         {
            asm volatile("cli");
         }
         break;
#endif
   }
}
