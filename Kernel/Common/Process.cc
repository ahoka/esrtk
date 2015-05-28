#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>
#include <Kernel/ProcessContext.hh>

using namespace Kernel;

Process::Process()
   : contextM(new ProcessContext)
{
}

Process::Process(uintptr_t pd)
   : contextM(new ProcessContext(pd))
{
}

Process::~Process()
{
   // XXX delete threads
   delete contextM;
}

bool
Process::createThread()
{
   printf("Process::createThread\n");

   auto thread = Thread::createUserThread(this);
   threadsM.push_back(thread);
   return thread->init();   
}

ProcessContext*
Process::getContext() const
{
   return contextM;
}
