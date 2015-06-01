#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Process.hh>
#include <Kernel/Watchdog.hh>
#include <Kernel/ProcessContext.hh>
#include <X86/PageDirectory.hh>

#include <cstdio>
#include <list>

#include <spinlock.h>

using namespace Kernel;

Thread* currentThread = 0;

static spinlock_softirq_t schedulerLock = SPINLOCK_SOFTIRQ_STATIC_INITIALIZER;

namespace
{
   Process* currentProcess = 0;
   Process* kernelProcess = 0;

   std::list<Thread*>* readyList;
   std::list<Thread*>* idleList;

};

Thread*
Scheduler::getIdleThread()
{
   static Thread thread0(Kernel::Thread::KernelThread);
   return &thread0;
}

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
Scheduler::setCurrentThread(Thread* thread)
{
   currentThread = thread;
}

Thread*
Scheduler::getCurrentThread()
{
   return currentThread;
}

Process*
Scheduler::getKernelProcess()
{
   return kernelProcess;
}

void
Scheduler::init()
{
   static std::list<Thread*> readyListInstance;
   static std::list<Thread*> idleListInstance;

   readyList = &readyListInstance;
   idleList = &idleListInstance;

   static Process process0(PageDirectory::getKernelPageDirectory());
   kernelProcess = &process0;

   Thread* thread0 = getIdleThread();
   thread0->init0(KernelStackStart);

   setCurrentThread(thread0);
   setCurrentProcess(&process0);

   process0.getContext()->switchContext();

   readyList->push_back(thread0);
}

void
Scheduler::insert(Thread* t)
{
   printf("Scheduler: inserting thread %lu\n", t->getId());

   spinlock_softirq_enter(&schedulerLock);

   readyList->push_back(t);

   spinlock_softirq_exit(&schedulerLock);
}

void
Scheduler::remove(Thread* t)
{
   printf("Scheduler: removing thread %lu\n", t->getId());

   spinlock_softirq_enter(&schedulerLock);

   readyList->remove(t);

   // XXX
   if (t == getCurrentThread())
   {
      printf("Removing currently running thread: %p\n", t);
      readyList->remove(getIdleThread());
      setCurrentThread(getIdleThread());
   }

   spinlock_softirq_exit(&schedulerLock);
}

void
Scheduler::schedule()
{
   spinlock_softirq_enter(&schedulerLock);

   // move interrupted thread to end of ready list
   Thread* lastRunning = getCurrentThread();
   readyList->push_back(lastRunning);

   // schedule the first thread in the ready list
   KASSERT(readyList->size() > 0);
   Thread* next = readyList->front();
   KASSERT(next != 0);
   readyList->pop_front();
   setCurrentThread(next);
   setCurrentProcess(next->getProcess());

   spinlock_softirq_exit(&schedulerLock);

   Watchdog::kick();
}
