#include <cstdio>

#include <Time.hh>

uint64_t Time::uptime = 0;

void
Time::increment(unsigned long ms)
{
   uptime += ms;
}

uint64_t
Time::getUptime()
{
   return uptime;
}
