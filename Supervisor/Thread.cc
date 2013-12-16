#include <Parameters.hh>
#include <Scheduler.hh>
#include <Debug.hh>

#include <Thread.hh>

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
   Scheduler::insert(this);
}

bool
Thread::init(unsigned long threadId, uintptr_t stack)
{
   id = threadId;
   kernelStack = stack;

   Debug::verbose("Initializing thread: %lu %p...\n", threadId, (void*)stack);

   return true;
}

bool
Thread::init()
{
   KASSERT(nextThreadId != 0);
   id = nextThreadId++;

   bool success = Memory::createKernelStack(kernelStack);
   KASSERT(success);

   Debug::verbose("Initializing thread...\n");

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
