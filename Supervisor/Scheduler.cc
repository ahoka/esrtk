#include <Supervisor/Scheduler.hh>
#include <Kernel/Thread.hh>

#include <cstdio>

namespace Kernel
{
   Thread thread0;

   Thread* threads = 0;
   Thread* currentThread = 0;
   Thread* nextToRun = 0;
};

using namespace Kernel;

void
Scheduler::init()
{
   thread0.init(0, StackStart);
}

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
