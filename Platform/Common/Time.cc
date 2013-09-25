#include <Time.hh>

uint64_t Time::uptime = 0;

void
Time::increment(unsigned long ms)
{
   Time::uptime += ms;
}
