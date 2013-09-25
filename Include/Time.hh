#ifndef TIME_HH
#define TIME_HH

#include <cstdint>

class Time
{
public:


private:
   Time();
   ~Time();

   friend class SystemTimer;
   static void increment(unsigned long ms);

   // system time in ms
   static uint64_t uptime;
};

#endif
