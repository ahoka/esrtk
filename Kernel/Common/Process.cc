#include <Kernel/Process.hh>
#include <Kernel/Thread.hh>

using namespace Kernel;

Process::Process()
   : pageDirectoryM(0)
{
}

Process::~Process()
{
}
