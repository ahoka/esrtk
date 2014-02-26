#include <Scheduler.hh>
#include <Thread.hh>

#include <cstdio>

Thread* Scheduler::currentThread = 0;

Thread* Scheduler::threads = 0;
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
   if (threads == 0) {
      printf("setting thread zero\n");
      t->dump();
      setCurrentThread(t);
      getCurrentThread()->dump();
   }

   t->next = threads;
   threads = t;
}

void
Scheduler::schedule()
{
   printf("sched\n");

   if (nextToRun == 0)
   {
      nextToRun = threads;
   }

   KASSERT(threads != 0);

   currentThread = nextToRun;
   nextToRun = nextToRun->next;

   currentThread->dump();
}
