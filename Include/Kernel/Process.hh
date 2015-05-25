#ifndef PROCESS_HH
#define PROCESS_HH

#include <Kernel/Thread.hh>
#include <list>
#include <cstdint>

namespace Kernel
{
   class ProcessContext;
   
   class Process final
   {
   public:
      Process();
      ~Process();

   private:
      ProcessContext* contextM;
      std::list<Thread*> threadsM;
   };
};

#endif
