#include <Scheduler.hh>
#include <Thread.hh>

#include <cstdio>

static Thread thread0;

Thread* Scheduler::threads = 0;
Thread* Scheduler::currentThread = 0;
Thread* Scheduler::nextToRun = 0;

void
Scheduler::setCurrentThread(Thread* thread)
{
   currentThread = thread;
}

Thread*
Scheduler::getCurrentThread()
{
   return currentThread;
}

void
Scheduler::insert(Thread* t)
{
   // XXX
   if (threads == 0) {
      setCurrentThread(t);
   }

   t->next = threads;
   threads = t;
}

void
Scheduler::schedule()
{
   if (nextToRun == 0)
   {
      nextToRun = threads;
   }

   KASSERT(threads != 0);

   currentThread = nextToRun;
   nextToRun = nextToRun->next;
}
