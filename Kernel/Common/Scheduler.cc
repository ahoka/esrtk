#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Watchdog.hh>

#include <cstdio>

#include <DoublyLinkedList.hh>

namespace Kernel
{
   Thread thread0;

//   using ThreadQueue = DoublyLinkedList<Thread>;
   Thread* idleListHeadM;
   Thread* idleListTailM;
   Thread* readyListHeadM;
   Thread* readyListTailM;
//   ThreadQueue* idleListM;
//   ThreadQueue* readyListM;

//   Thread* threads = 0;
   Thread* currentThread = 0;
//   Thread* nextToRun = 0;
};

using namespace Kernel;

void
Scheduler::init()
{
   // readyListM = new ThreadQueue();
   // idleListM = new ThreadQueue();

   thread0.init(0, StackStart);

   setCurrentThread(&thread0);

   readyListHeadM = &thread0;
   readyListTailM = &thread0;

//   idleListM = 0;
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
   // if (nextToRun == 0)
   // {
   //    nextToRun = threads;
   // }

//   KASSERT(threads != 0);

   // currentThread = readyListHeadM;
   // readyListHeadM = readyListHeadM->nextM;
   // if (readyListHeadM == 0)
   // {
   //    readyListTailM = 0;
   // }

   // // XXX no idle yet, just rr threads
   // readyListM->push(currentThread);
//   currentThread = nextToRun;
//   nextToRun = nextToRun->next;

   Watchdog::kick();
}
