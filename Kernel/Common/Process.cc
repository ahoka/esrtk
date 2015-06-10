#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>
#include <Kernel/ProcessContext.hh>

using namespace Kernel;

namespace
{
   unsigned long nextId = 0;
};

std::list<Process*>&
getProcessList()
{
   static std::list<Process*> processList;

   return processList;
}

Process::Process()
   : idM(nextId++),
     contextM(new ProcessContext)
{
   printf("Process creation: %p\n", this);
   getProcessList().push_back(this);
}

Process::Process(uintptr_t pd)
   : contextM(new ProcessContext(pd))
{
   printf("Process creation: %p (existing pd: %p)\n", this, (void*)pd);
}

Process::~Process()
{
   printf("Process deletion: %p\n", this);
   
   for (auto& t : threadsM)
   {
      delete t;
   }
   
   delete contextM;
}

void
Process::dump()
{
   printf(" %lu\n", idM);
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
