#ifndef CLOCK_HH
#define CLOCK_HH

#include <cstdint>

class ClockProvider;

class Clock
{
public:
   Clock();
   ~Clock();

   static uint64_t getTime();

   static void registerProvider(ClockProvider* provider);

private:
   static ClockProvider* clocks;
   static ClockProvider* activeClock;

   friend class Platform;
   static void probeAndInit();

   friend class Time;
};

#endif
