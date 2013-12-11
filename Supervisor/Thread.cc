#include <Parameters.hh>
#include <Scheduler.hh>

#include <Thread.hh>

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
   printf("Creating thread...\n");
   Scheduler::insert(this);
}

bool
Thread::init(unsigned long threadId, uintptr_t stack)
{
   id = threadId;
   kernelStack = stack;

   printf("Initializing thread: %lu %p...\n", threadId, (void*)stack);

   return true;
}

bool
Thread::init()
{
   KASSERT(nextThreadId != 0);
   id = nextThreadId++;

   bool success = Memory::createKernelStack(kernelStack);
   KASSERT(success);

   printf("Initializing thread...\n");

   return success;
}

void
Thread::printAll()
{
   // for (auto& t : list)
   // {
   //    printf("%lu - %p\n", t.id, (void*)t.kernelStack);
   // }
}
