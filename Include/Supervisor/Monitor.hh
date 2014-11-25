#ifndef MONITOR_HH
#define MONITOR_HH

#include <string>

class Monitor
{
public:
   Monitor();
   ~Monitor();

   std::string getCommand();
   void enter();
};

#endif
