#include <Supervisor/Scheduler.hh>
#include <Kernel/Thread.hh>
#include <X86/ThreadContext.hh>

#include <Parameters.hh>
#include <Debug.hh>
#include <Hal.hh>

#include <cstdio>

using namespace Kernel;

unsigned long Thread::nextThreadId = 1;

Thread::Thread()
   : id(-1ul),
     kernelStack(0),
     stateM(Idle),
     next(0)
{
   Debug::verbose("Creating thread...\n");
}

Thread::Thread(unsigned long Id)
   : id(Id),
     kernelStack(0),
     stateM(Idle),
     next(0)
{
}

// used when creating thread 0
bool
Thread::init(unsigned long threadId, uintptr_t stack)
{
   id = threadId;
   kernelStack = stack;

   Debug::verbose("Initializing idle thread: %lu %p...\n", threadId, (void*)stack);

   Scheduler::insert(this);

   return true;
}

bool
Thread::init()
{
   Debug::verbose("Initializing thread %p...\n", this);

   InterruptFlags flags;
   Hal::saveLocalInterrupts(flags);
   Hal::disableLocalInterrupts();

   KASSERT(nextThreadId != 0);
   id = nextThreadId++;

   bool success = Memory::createKernelStack(kernelStack);
   kernelStack = ThreadContext::initStack(kernelStack,
                                          reinterpret_cast<uintptr_t>(&Thread::main),
                                          reinterpret_cast<uintptr_t>(this));
   Hal::restoreLocalInterrupts(flags);
   KASSERT(success);

   Debug::verbose("Thread's new stack is %p\n", (void*)kernelStack);
   if (success)
   {
      Scheduler::insert(this);
   }

   return success;
}

bool
Thread::addJob(Job job)
{
   jobsM.push(job);

   return true;
}

void
Thread::printAll()
{
   // for (auto& t : list)
   // {
   //    Debug::verbose("%lu - %p\n", t.id, (void*)t.kernelStack);
   // }
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
         while (!thread->jobsM.empty())
         {
            Job job = thread->jobsM.front();
            thread->jobsM.pop();

            job.execute();
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
   printf("thread: %p %lu %p %u\n", this, id, (void *)kernelStack, stateM);
}

Thread*
Thread::create()
{
   Thread* thread = new Thread();
   thread->init();

   return thread;
}
