#ifndef PROCESS_HH
#define PROCESS_HH

#include <Kernel/Thread.hh>
#include <list>
#include <cstdint>

namespace Kernel
{
   class ProcessContext;
   class Scheduler;
   
   class Process final
   {
   public:
      ~Process();

      Thread* createThread();
      ProcessContext* getContext() const;
      uint64_t getId() const;

      void dump();

      static Process* createProcess();
      
      static void printAll();

     private:
      Process();
      explicit Process(uintptr_t);

      uint64_t idM;
      ProcessContext* contextM;
      std::list<Thread*> threadsM;

      friend class ::Kernel::Scheduler;
   };
};

#endif
