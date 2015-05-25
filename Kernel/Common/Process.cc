#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>
#include <Kernel/ProcessContext.hh>

using namespace Kernel;

Process::Process()
   : contextM(new ProcessContext)
{
   Thread* thread = Thread::createUserThread(this);
   threadsM.push_back(thread);
   thread->init();
}

Process::~Process()
{
   // XXX delete threads
   delete contextM;
}
