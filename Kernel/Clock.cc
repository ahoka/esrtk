#include <Debug.hh>

#include <Clock.hh>
#include <ClockProvider.hh>

#include <cstdio>
#include <cassert>

ClockProvider* Clock::clocks = 0;
ClockProvider* Clock::activeClock = 0;

Clock::Clock()
{
}

Clock::~Clock()
{
}

void
Clock::registerProvider(ClockProvider* provider)
{
   Debug::verbose("Clock::registerProvider(%p)\n", provider);
   provider->nextClock = clocks;
   clocks = provider;
}

void
Clock::probeAndInit()
{
   Debug::verbose("Clock::probeAndInit()\n");
   ClockProvider* c = clocks;
   ClockProvider* best = 0;
   int bestProbe = 0;

   while (c != 0)
   {
      KASSERT(c != 0);
      int probeResult = c->probe();

      if (c->probe() > bestProbe)
      {
         bestProbe = probeResult;
         best = c;
      }

      c = c->nextClock;
   }

   assert(best != 0);

   best->startClock();
   activeClock = best;

   Debug::info("Selecting %s as best clock source\n", best->getName());
}

uint64_t
Clock::getTime()
{
   if (activeClock == 0)
   {
      return 0;
   }
   else
   {
      auto freq = activeClock->getFrequency();

      KASSERT(freq != 0);

      return activeClock->getValue() / freq;
   }
}
