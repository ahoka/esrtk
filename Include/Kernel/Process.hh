#ifndef PROCESS_HH
#define PROCESS_HH

#include <Kernel/Thread.hh>

#include <cstdint>
#include <ustl/ulist.h>

namespace Kernel
{
   class Process final
   {
   public:
      Process();
      ~Process();

   private:
      uint32_t pageDirectoryM;

      ustl::list<Thread*> threadsM;
   };
};

#endif
