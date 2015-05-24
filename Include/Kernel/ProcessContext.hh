#ifndef PROCESSCONTEXT_HH
#define PROCESSCONTEXT_HH

#include <cstdint>

namespace Kernel
{
   namespace ProcessContext
   {
      void init();
      void setKernelStack(uintptr_t stack);
   };
};

#endif

