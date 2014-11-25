#include <Supervisor/Monitor.hh>

#include <Time.hh>
#include <Power.hh>
#include <MemoryManager.hh>
#include <Kernel/Thread.hh>
#include <Interrupt.hh>

#include <string>
#include <list>
#include <cstdio>
#include <utility>
#include <functional>

using CommandPair = std::pair<MonitorCommand*, std::string>;

MonitorCommand::MonitorCommand()
{
}

MonitorCommand::~MonitorCommand()
{
}

class InterruptCommand : public MonitorCommand
{
public:
   void run(std::string)
   {
      printf("Interrupt statistics:\n");
      Interrupt::printStatistics();
   }
};

class UptimeCommand : public MonitorCommand
{
public:
   void run(std::string)
   {
      uint64_t uptime = Time::getUptime();
      printf("Uptime is: %lu.%lu\n",
             (unsigned long )uptime / 1000,
             (unsigned long )(uptime % 1000));
   }
};

class LambdaCommand : public MonitorCommand
{
public:
   using CommandFunction = std::function<void(std::string)>;

   LambdaCommand(CommandFunction lambda, std::string command)
      : lambdaM(lambda)
   {
      Monitor::registerCommand(this, command);
   }

   void run(std::string args)
   {
      lambdaM(args);
   }

private:
   CommandFunction lambdaM;
};

Monitor::Monitor()
{
//   static InterruptCommand interruptCommand;
   static UptimeCommand uptimeCommand;

//   Monitor::registerCommand(&interruptCommand, "interrupts");
   Monitor::registerCommand(&uptimeCommand, "uptime");

   static LambdaCommand interruptCommand([](std::string) {
         printf("Interrupt statistics:\n");
         Interrupt::printStatistics();
   }, "interrupts");
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

namespace
{
   std::list<CommandPair> commands;
};

void
Monitor::registerCommand(MonitorCommand* command, std::string name)
{
   auto pair =  std::make_pair(command, name);

   commands.push_back(pair);
}

void
Monitor::enter()
{
   printf("\nEntering ESRTK Monitor\n\n");

   bool isRunning = true;
   while (isRunning)
   {
      std::string command = getCommand();

      if (command == "help")
      {
         printf("Available commands:\n");
         for (auto& handler : commands)
         {
            printf(" %s\n", handler.second.c_str());
         }
      }
      else
      {
         for (auto& handler : commands)
         {
            if (handler.second == command)
            {
               handler.first->run(command);
            }
         }
      }

      if (command == "memory")
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
