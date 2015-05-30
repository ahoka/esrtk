#include <Kernel/SystemCall.hh>

#include <cstdio>

using namespace Kernel;

void
Kernel::handleSystemCall(unsigned int syscall, void* arg)
{
   printf("syscall: %u: %p (%zu)\n", syscall, arg);
}
