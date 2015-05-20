#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>
#include <X86/PageDirectory.hh>

using namespace Kernel;

Process::Process()
   : pageDirectoryM(PageDirectory::createPageDirectory())
{
   printf("Process: PD: %p\n", (void*)pageDirectoryM);

   Thread* thread = new Thread;
   threadsM.push_back(thread);
   thread->init();
}

Process::~Process()
{
}
