#include <Supervisor/Scheduler.hh>
#include <Kernel/Thread.hh>

#include <cstdio>

#include <ustl/uqueue.h>

namespace Kernel
{
   Thread thread0;

   using ThreadQueue = ustl::queue<Thread*>;
   ThreadQueue* idleListM;
   ThreadQueue* readyListM;

//   Thread* threads = 0;
   Thread* currentThread = 0;
//   Thread* nextToRun = 0;
};

using namespace Kernel;

void
Scheduler::init()
{
   readyListM = new ThreadQueue();
   thread0.init(0, StackStart);
   printf("push\n");
   readyListM->push(&thread0);
   printf("push-\n");
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
   printf("ins\n");
   // XXX
   // if (threads == 0)
   // {
   //    setCurrentThread(t);
   // }

//   t->next = threads;
//   threads = t;
   readyListM->push(t);

   printf("ins-\n");
}

void
Scheduler::schedule()
{
   // if (nextToRun == 0)
   // {
   //    nextToRun = threads;
   // }

//   KASSERT(threads != 0);

   printf("sch\n");
   currentThread = readyListM->front();
   readyListM->pop();

   // XXX no idle yet, just rr threads
   readyListM->push(currentThread);
//   currentThread = nextToRun;
//   nextToRun = nextToRun->next;
}
