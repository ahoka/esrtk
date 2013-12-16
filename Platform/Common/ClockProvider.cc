#include <Clock.hh>
#include <ClockProvider.hh>

ClockProvider::ClockProvider()
{
   Clock::registerProvider(this);
}

ClockProvider::~ClockProvider()
{
   // XXX remove timer and run
   // probeAndInit();
}
