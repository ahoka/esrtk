#ifndef PROCESS_HH
#define PROCESS_HH

#include <Kernel/Thread.hh>

#include <cstdint>

namespace Kernel
{
   class Process final
   {
   public:
      Process();
      ~Process();

   private:
      uint32_t pageDirectoryM;

      Thread* threadM;
   };
};

#endif
