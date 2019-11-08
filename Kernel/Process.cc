#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>
#include <Kernel/ProcessContext.hh>
#include <Mutex.hh>

using namespace Kernel;

namespace
{
   volatile uint64_t nextId = 0;

   uint64_t getNextId()
   {
      return __sync_fetch_and_add(&nextId, 1);
   }

   Mutex processListLock;
};

std::list<Process*>&
getProcessList()
{
   static std::list<Process*> processList;

   return processList;
}

Process::Process()
   : idM(getNextId()),
     contextM(new ProcessContext)
{
   printf("Process creation: %p, PID: %llu\n", this, idM);
   processListLock.enter();
   getProcessList().push_back(this);
   processListLock.exit();
}

Process::Process(uintptr_t pd)
   : idM(getNextId()),
     contextM(new ProcessContext(pd))
{
   printf("Process creation: %p (existing pd: %p)\n", this, (void*)pd);
   processListLock.enter();
   getProcessList().push_back(this);
   processListLock.exit();
}

Process*
Process::createProcess()
{
   return new Process();
}

Process::~Process()
{
   printf("Process deletion: %p, PID: %llu\n", this, idM);

   processListLock.enter();
   getProcessList().remove(this);
   processListLock.exit();


   for (auto& t : threadsM)
   {
      delete t;
   }

   delete contextM;
}

void
Process::dump()
{
   printf(" %llu\n", idM);
}

void
Process::printAll()
{
   for (auto& p : getProcessList())
   {
      p->dump();
   }
}

Thread*
Process::createThread()
{
   printf("Process::createThread\n");

   auto thread = Thread::createUserThread(this);
   threadsM.push_back(thread);

   return thread;
}

ProcessContext*
Process::getContext() const
{
   return contextM;
}

uint64_t
Process::getId() const
{
   return idM;
}
