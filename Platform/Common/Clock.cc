#include <Clock.hh>

Clock* Clock::clocks = 0;
Clock* Clock::activeClock = 0;

Clock::Clock()
{
   nextClock = clocks;
   clocks = this;
}

Clock::~Clock()
{
   // XXX remove timer and run
   // probeAndInit();
}

void
Clock::probeAndInit()
{
   Clock* c = clocks;
   Clock* best = 0;
   int bestProbe = 0;

   while (c != 0)
   {
      int probeResult = c->probe();

      if (c->probe() > bestProbe)
      {
         bestProbe = probeResult;
         best = c;
      }

      c = c->nextClock;
   }

   best->startClock();
   activeClock = best;
}

uint64_t
Clock::getTimeValue()
{
   if (activeClock == 0)
   {
      return 0;
   }
   else
   {
      return activeClock->getTime();
   }
}
