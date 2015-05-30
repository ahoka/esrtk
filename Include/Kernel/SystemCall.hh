#ifndef SYSTEMCALL_HH
#define SYSTEMCALL_HH

#include <cstdlib>

namespace Kernel
{
   void handleSystemCall(unsigned int syscall, void* arg);
};

#endif
