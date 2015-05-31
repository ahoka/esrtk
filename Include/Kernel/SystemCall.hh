#ifndef SYSTEMCALL_HH
#define SYSTEMCALL_HH

#include <cstdlib>

namespace Kernel
{
   int handleSystemCall(unsigned int syscall, void* arg);
};

#endif
