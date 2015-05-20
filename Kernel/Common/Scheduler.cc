#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Watchdog.hh>

#include <cstdio>

#include <DoublyLinkedList.hh>

namespace Kernel
{
   Thread* idleListHeadM;
   Thread* idleListTailM;
   Thread* readyListHeadM;
   Thread* readyListTailM;
   Thread* currentThread = 0;
};

using namespace Kernel;

void
Scheduler::init()
{
   static Thread thread0;

   thread0.init0(StackStart);

   setCurrentThread(&thread0);

   readyListHeadM = &thread0;
   readyListTailM = &thread0;

   idleListHeadM = 0;
   idleListTailM = 0;
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
   // if (threads == 0)
   // {
   //    setCurrentThread(t);
   // }

//   t->next = threads;
//   threads = t;

   if (readyListTailM == 0)
   {
      assert(readyListHeadM == 0);
      assert(t->nextM == 0);

      readyListTailM = t;
      readyListHeadM = t;
   }
   else
   {
      Thread* oldTail = readyListTailM;
      readyListTailM = t;

      oldTail->nextM = t;
//      t->prevM(oldTail);
   }
}

void
Scheduler::schedule()
{
   Thread* lastRunning = getCurrentThread();
   
   if (readyListTailM)
   {
      readyListTailM->nextM = lastRunning;
   }
   
   readyListTailM = lastRunning;
   
   Thread* next = readyListHeadM;

   KASSERT(next != 0);

   readyListHeadM = readyListHeadM->nextM;

   setCurrentThread(next);

   Watchdog::kick();
}
