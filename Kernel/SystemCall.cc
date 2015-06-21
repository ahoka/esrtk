#include <Kernel/SystemCall.hh>

#include <cstdio>

using namespace Kernel;

int
Kernel::handleSystemCall(unsigned int syscall, void* arg)
{
   printf("syscall: %u: %p\n", syscall, arg);

   return 0;
}
