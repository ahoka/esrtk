#include <cstdio>

#include <Time.hh>
#include <Clock.hh>

uint64_t Time::uptime = 0;

void
Time::increment(unsigned long ms)
{
   uptime += ms;
}

uint64_t
Time::getUptime()
{
   uint64_t time = Clock::getTime();

//   printf("clock: %lu, jiffies: %lu\n", (unsigned long)uptime, (unsigned long)time);

   if (time == 0)
   {
      return uptime;
   }
   else
   {
      return time;
   }
}
