#include <SystemTimer.hh>

#include <Kernel/Scheduler.hh>
#include <Kernel/Watchdog.hh>

#include <Time.hh>
#include <Debug.hh>

#include <cstdio>
#include <cassert>

SystemTimer* SystemTimer::timers = 0;

SystemTimer::SystemTimer()
{
   nextTimer = timers;
   timers = this;
}

SystemTimer::~SystemTimer()
{
   // XXX remove timer
}

void
SystemTimer::tick()
{
   KASSERT(getFrequency() <= 500);
   Time::increment(1000 / getFrequency());

   Kernel::Scheduler::schedule();
   Watchdog::kick();
}

void
SystemTimer::probeAndInit()
{
   SystemTimer* t = timers;
   SystemTimer* best = 0;
   int bestProbe = 0;

   while (t != 0)
   {
      int probeResult = t->probe();

      if (t->probe() > bestProbe)
      {
         bestProbe = probeResult;
         best = t;
      }

      t = t->nextTimer;
   }

   assert(best != 0);

   best->startTimer();
}
