#include <Supervisor/Scheduler.hh>
#include <Kernel/Thread.hh>

#include <Parameters.hh>
#include <Debug.hh>
#include <Hal.hh>

#include <cstdio>

unsigned long Thread::nextThreadId = 1;

// for creating thread0
//
Thread::Thread()
   : id(-1ul),
     kernelStack(0),
     state(Idle),
     next(0)
{
   Debug::verbose("Creating thread...\n");
}

Thread::Thread(unsigned long Id)
   : id(Id),
     kernelStack(0),
     state(Idle),
     next(0)
{
}

bool
Thread::init(unsigned long threadId, uintptr_t stack)
{
   id = threadId;
   kernelStack = stack;

   Debug::verbose("Initializing thread: %lu %p...\n", threadId, (void*)stack);

   Scheduler::insert(this);

   return true;
}

bool
Thread::init()
{
   Debug::verbose("Initializing thread...\n");

   InterruptFlags flags;
   Hal::saveLocalInterrupts(flags);
   Hal::disableLocalInterrupts();

   KASSERT(nextThreadId != 0);
   id = nextThreadId++;

   bool success = Memory::createKernelStack(kernelStack);
   Hal::restoreLocalInterrupts(flags);
   KASSERT(success);

   Debug::verbose("Thread's new stack is %p\n", (void*)kernelStack);

   if (success) {
      Scheduler::insert(this);
   }

   return success;
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
Thread::main()
{
   Debug::verbose("Thread main called!\n");

   for (;;)
   {
      asm volatile("pause");
   }
}

void
Thread::dump()
{
   printf("thread: %p %lu %p %u\n", this, id, (void *)kernelStack, state);
}
