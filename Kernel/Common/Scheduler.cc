#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Process.hh>
#include <Kernel/Watchdog.hh>

#include <cstdio>

#include <spinlock.h>

using namespace Kernel;

Thread* idleListHeadM = 0;
Thread* idleListTailM = 0;
Thread* readyListHeadM = 0;
Thread* readyListTailM = 0;
Thread* currentThread = 0;

static spinlock_softirq_t schedulerLock = SPINLOCK_SOFTIRQ_STATIC_INITIALIZER;

namespace
{
   Process* currentProcess = 0;
};

void
Scheduler::setCurrentProcess(Process* process)
{
   currentProcess = process;
}

Process*
Scheduler::getCurrentProcess()
{
   return currentProcess;
}

void
Scheduler::init()
{
   static Thread thread0(Kernel::Thread::KernelThread);
//   static Process process0;

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
   spinlock_softirq_enter(&schedulerLock);

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
   }

   spinlock_softirq_exit(&schedulerLock);
}

void
Scheduler::schedule()
{
   spinlock_softirq_enter(&schedulerLock);
   
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

   spinlock_softirq_exit(&schedulerLock);

   Watchdog::kick();
}
