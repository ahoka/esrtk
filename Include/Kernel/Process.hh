#ifndef PROCESS_HH
#define PROCESS_HH

#include <Kernel/Thread.hh>
#include <list>
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

      std::list<Thread*> threadsM;
   };
};

#endif
