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
   static LambdaCommand interruptCommand([](std::string) {
         printf("Interrupt statistics:\n");
         Interrupt::printStatistics();
   }, "interrupts");

   static LambdaCommand uptimeCommand([](std::string) {
         uint64_t uptime = Time::getUptime();
         printf("Uptime is: %lu.%lu\n",
                (unsigned long )uptime / 1000,
                (unsigned long )(uptime % 1000));
   }, "uptime");

   static LambdaCommand memoryCommand([](std::string) {
         MemoryManager::get().printStatistics();
   }, "memory");

   static LambdaCommand threadsCommand([](std::string) {
         Kernel::Thread::printAll();         
   }, "threads");

   static LambdaCommand rebootCommand([](std::string) {
         Power::reboot();
   }, "reboot");

   static LambdaCommand deadlockCommand([](std::string) {
         printf("Simulating a dead-lock!\n");
         for (;;)
         {
            asm volatile("cli");
         }
   }, "deadlock");
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
      else if (command == "exit")
      {
         isRunning = false;
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
   }
}
