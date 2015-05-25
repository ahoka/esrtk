#ifndef PROCESSCONTEXT_HH
#define PROCESSCONTEXT_HH

#include <cstdint>

namespace Kernel
{
   class ProcessContext
   {
   public:
      static void init();
      static void setKernelStack(uintptr_t stack);

      ProcessContext();
      void switchContext();

   private:
      uintptr_t pageDirectoryM;
   };
};

#endif

