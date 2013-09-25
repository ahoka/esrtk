#include <Debug.hh>

#include <SystemTimer.hh>
#include <Scheduler.hh>
#include <Time.hh>

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
   Scheduler::tick();
   KASSERT(getFrequency() <= 500);
   Time::increment(1000 / getFrequency());
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

   best->startTimer();
}
