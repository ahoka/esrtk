#include <Scheduler.hh>
#include <Thread.hh>

#include <cstdio>

static Thread* currentThread = 0;

Thread* Scheduler::threads = 0;
Thread* Scheduler::nextToRun = 0;

void
Scheduler::insert(Thread* t)
{
   t->next = threads;
   threads = t;
}

// running in interrupt context
void
Scheduler::tick()
{
   if (nextToRun == 0)
   {
      nextToRun = threads;
   }

   KASSERT(threads != 0);

   currentThread = nextToRun;
   nextToRun = nextToRun->next;
}

void
Scheduler::dispatch()
{
}
