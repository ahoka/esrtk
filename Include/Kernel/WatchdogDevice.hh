#ifndef WATCHDOGDEVICE_HH
#define WATCHDOGDEVICE_HH

#include <Watchdog.hh>

class WatchdogDevice
{
public:
   virtual void kick() = 0;

   bool registerWatchdog();
   bool unregisterWatchdog();

private:
   
   WatchdogDevice* nextWatchdogDeviceM;

   friend void Watchdog::kick();
};

#endif
