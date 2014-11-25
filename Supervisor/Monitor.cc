#include <Supervisor/Monitor.hh>

#include <Time.hh>
#include <Power.hh>
#include <MemoryManager.hh>
#include <Kernel/Thread.hh>
#include <Interrupt.hh>

#include <string>
#include <cstdio>

Monitor::Monitor()
{
}

Monitor::~Monitor()
{
}

std::string
Monitor::getCommand()
{
   char buffer[128];

   printf("=> ");
   std::string command = gets_s(buffer, sizeof(buffer));

   return command;
}

void
Monitor::enter()
{
   printf("\nEntering ESRTK Monitor\n\n");

   bool isRunning = true;
   while (isRunning)
   {
      std::string command = getCommand();

      if (command == "interrupts")
      {
         printf("Interrupt statistics:\n");
         Interrupt::printStatistics();
      }
      else if (command == "time")
      {
         uint64_t uptime = Time::getUptime();
         printf("Uptime is: %lu.%lu\n",
                (unsigned long )uptime / 1000,
                (unsigned long )(uptime % 1000));
      }
      else if (command == "memory")
      {
         putchar('\n');
         MemoryManager::get().printStatistics();
      }
      else if (command == "threads")
      {
         putchar('\n');
         Kernel::Thread::printAll();
      }
      else if (command == "reboot")
      {
         Power::reboot();
      }
      else if (command == "deadlock")
      {
         printf("Simulating a dead-lock!\n");
         for (;;)
         {
            asm volatile("cli");
         }
      }
      else if (command == "exit")
      {
         isRunning = false;
      }
   }
}
