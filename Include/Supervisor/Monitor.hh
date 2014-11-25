#ifndef MONITOR_HH
#define MONITOR_HH

#include <string>

class MonitorCommand
{
public:
   MonitorCommand();
   virtual ~MonitorCommand();

   virtual void run(std::string args) = 0;
};

class Monitor
{
public:
   Monitor();
   ~Monitor();

   static void registerCommand(MonitorCommand* command, std::string name);
   std::string getCommand();
   void enter();
};

#endif
