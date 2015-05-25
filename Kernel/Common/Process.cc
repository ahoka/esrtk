#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>
#include <X86/PageDirectory.hh>

using namespace Kernel;

Process::Process()
{
   Thread* thread = Thread::createUserThread();
   threadsM.push_back(thread);
   thread->init();
}

Process::~Process()
{
}
