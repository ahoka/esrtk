#ifndef __THREAD_HH__
#define __THREAD_HH__

#include <Memory.hh>
#include <Kernel/Job.hh>

#include <DoublyLinkedList.hh>
#include <cstdint>

namespace Kernel
{
   class Process;
   
   class Thread
   {
   public:
      Thread();
//      Thread(unsigned long);

      bool init();
      bool init0(uintptr_t stack);

      bool addJob(Job job);

      void dump();

      static void printAll();
      static void main [[noreturn]] (Thread* thread);

      static Thread* create();

      enum State
      {
         New,
         Idle,
         Ready,
         Running,
         Agony,
         Dead
      };

      enum Type
      {
         UserThread,
         KernelThread,
         InterruptThread
      };
   
      unsigned long id;
//   uintptr_t userStack;
      uintptr_t kernelStack;
      State stateM;
      Type typeM;

      Process* processM;

      Thread* nextM;

   private:
//      DoublyLinkedList<Job> jobsM;

      static unsigned long nextThreadId;
   };
};

#endif
