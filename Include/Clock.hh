#ifndef CLOCK_HH
#define CLOCK_HH

#include <cstdint>

class Clock
{
public:
   Clock();
   virtual ~Clock();

private:
   virtual int probe() = 0;
   virtual bool startClock() = 0;
   virtual bool stopClock() = 0;
   virtual uint64_t getTime() = 0;

   static Clock* clocks;
   static Clock* activeClock;

   Clock* nextClock;

   friend class Platform;
   static void probeAndInit();

   friend class Time;
   static uint64_t getTimeValue();
};

#endif
