#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>
#include <Kernel/ProcessContext.hh>

using namespace Kernel;

Process::Process()
   : contextM(new ProcessContext)
{
//   Thread* thread = Thread::createUserThread(this);
//   threadsM.push_back(thread);
//   thread->init();
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
