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
      explicit Process(uintptr_t);
      ~Process();

      bool createThread();
      ProcessContext* getContext() const;

     private:
      ProcessContext* contextM;
      std::list<Thread*> threadsM;
   };
};

#endif
