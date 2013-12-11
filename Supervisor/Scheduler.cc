#include <Scheduler.hh>
#include <Thread.hh>

Thread* Scheduler::threads = 0;

void
Scheduler::insert(Thread* t)
{
   t->next = threads;
   threads = t;
}

void
Scheduler::tick()
{
   // not yet
}

void
Scheduler::dispatch()
{
}
