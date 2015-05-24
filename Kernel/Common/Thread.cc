#include <Kernel/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <X86/ThreadContext.hh>

#include <Parameters.hh>
#include <Debug.hh>
#include <spinlock.h>

#include <list>

#include <cstdio>

using namespace Kernel;

namespace
{
   spinlock_softirq_t threadLock = SPINLOCK_SOFTIRQ_STATIC_INITIALIZER;

   std::list<Thread*>&
   getThreadList()
   {
      static std::list<Thread*> threadList;

      return threadList;
   }
};

unsigned long Thread::nextThreadId = 1;

Thread::Thread(Thread::Type type)
   : idM(-1ul),
     kernelStackM(0),
     stateM(Idle),
     typeM(type),
     processM(0),
     nextM(0)
{
   Debug::verbose("Creating thread...\n");
}

// used when creating thread 0
bool
Thread::init0(uintptr_t stack)
{
   idM = 0;
   kernelStackM = stack;

   Debug::verbose("Initializing idle thread (thread0): %p...\n", (void*)stack);

   Scheduler::insert(this);
   getThreadList().push_back(this);

   return true;
}

bool
Thread::init()
{
   Debug::verbose("Initializing thread %p...\n", this);

   spinlock_softirq_enter(&threadLock);

   KASSERT(nextThreadId != 0);
   idM = nextThreadId++;

   bool success = Memory::createKernelStack(kernelStackM);
   kernelStackM = ThreadContext::initStack(kernelStackM,
                                           reinterpret_cast<uintptr_t>(&Thread::main),
                                           reinterpret_cast<uintptr_t>(this));

   spinlock_softirq_exit(&threadLock);
   KASSERT(success);

   Debug::verbose("Thread's new stack is %p\n", (void*)kernelStackM);
   if (success)
   {
      Scheduler::insert(this);
      getThreadList().push_back(this);
   }

   return success;
}

bool
Thread::addJob(Job /*job*/)
{
//   jobsM.push(job);

   return true;
}

void
Thread::printAll()
{
   for (auto& t : getThreadList())
   {
      printf("%p (%lu) - %p\n", t, t->getId(), (void*)t->kernelStackM);
   }
}

void
Thread::main(Thread* thread)
{
   Debug::verbose("Thread main called on %p (%p)!\n", thread, &thread);

   for (;;)
   {
      if (thread->stateM == Ready)
      {
         thread->stateM = Running;
         // while (!thread->jobsM.empty())
         // {
         //    Job job = thread->jobsM.front();
         //    thread->jobsM.pop();

         //    job.execute();
         // }
         while (true) // XXX
         {
            asm volatile("pause");
         }
      }
      else if (thread->stateM == Agony)
      {
         printf("Thread %p is exiting...\n", thread);
         thread->stateM = Dead;
         for (;;)
         {
            // wait for destruction
            asm volatile("pause");
         }
      }

      thread->stateM = Idle;
   }
}

void
Thread::dump()
{
   printf("thread: %p %lu %p %u\n", this, idM, (void *)kernelStackM, stateM);
}

Thread*
Thread::createKernelThread()
{
   Thread* thread = new Thread(Type::KernelThread);

   return thread;
}

unsigned long
Thread::getId() const
{
   return idM;
}

void
Thread::setName(const char* name)
{
   nameM = std::string(name);
}

std::string
Thread::getName() const
{
   return nameM;
}

uintptr_t
Thread::getKernelStack() const
{
   return kernelStackM;
}

void
Thread::setKernelStack(uintptr_t stack)
{
   kernelStackM = stack;
}
