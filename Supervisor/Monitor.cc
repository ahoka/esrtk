#include <Supervisor/Monitor.hh>

#include <Time.hh>
#include <Power.hh>
#include <Kernel/Heap.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Process.hh>
#include <Interrupt.hh>
#include <Scheduler.hh>
#include <Memory.hh>

#include <string>
#include <list>
#include <cstdio>
#include <utility>
#include <functional>

using CommandPair = std::pair<MonitorCommand*, std::string>;

namespace
{
   std::list<CommandPair> commands;
   const char* prompt = ">";
};

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

   static LambdaCommand heapCommand([](std::string) {
         Kernel::Heap::get().printStatistics();
   }, "heap");

   static LambdaCommand threadsCommand([](std::string) {
         Kernel::Thread::printAll();
   }, "threads");

   static LambdaCommand processCommand([](std::string) {
         Kernel::Process::printAll();
   }, "processes");

   static LambdaCommand rebootCommand([](std::string) {
         Power::reboot();
   }, "reboot");

   static LambdaCommand memoryCommand([](std::string) {
         Memory::info();
   }, "memory");

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

bool
expand(char* part, size_t len)
{
   const char* expanded = 0;
   unsigned int count = 0;

   for (const auto& c : commands)
   {
      size_t partlen = strlen(part);
      const char* cmd = c.second.c_str();

      if (strlen(cmd) >= partlen &&
          strncmp(part, cmd, partlen) == 0)
      {
         count++;
         if (expanded == 0)
         {
            expanded = cmd;
         }
      }
   }

   if (count == 1 && expanded != 0)
   {
      snprintf(part, len, "%s", expanded);
      return true;
   }

   return false;
}

void
complete(const char* part)
{
   for (const auto& c : commands)
   {
      size_t partlen = strlen(part);
      const char* cmd = c.second.c_str();

      if (strlen(cmd) >= partlen &&
          strncmp(part, cmd, partlen) == 0)
      {
         printf("  %s\n", cmd);
      }
   }
}

std::string
Monitor::getCommand()
{
   char buffer[128];
   size_t i = 0;

   printf("%s ", prompt);

   while (i < sizeof(buffer) - 1)
   {
      int c = getchar();
      if (c == '?')
      {
         buffer[i] = 0;
         if (expand(buffer, sizeof(buffer)))
         {
            printf("%s", buffer + i);
            i = strlen(buffer);
            continue;
         }
         else
         {
            putchar('\n');
            complete(buffer);
            printf("%s %s", prompt, buffer);
         }
      }
      else if (c == '\n')
      {
         break;
      }
      else
      {
         buffer[i++] = c;
      }
   }

   return std::string(buffer, i);
}

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
            printf("  %s\n", handler.second.c_str());
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
