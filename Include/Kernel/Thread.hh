#ifndef __THREAD_HH__
#define __THREAD_HH__

#include <Memory.hh>
#include <Kernel/Job.hh>

#include <Kernel/Scheduler.hh>

#include <string>
#include <cstdint>

namespace Kernel
{
   class Process;
   
   class Thread
   {
   public:
      enum Type
      {
         UserThread,
         KernelThread,
         InterruptThread
      };
      
      Thread(Type);
//      Thread(unsigned long);

      bool init();
      bool init0(uintptr_t stack);
      bool addJob(Job job);
      void dump();

      void setName(const char*);
      std::string getName() const;

      void setKernelStack(uintptr_t);
      uintptr_t getKernelStack() const;

      unsigned long getId() const;

      static void printAll();
      static void main [[noreturn]] (Thread* thread);
      static Thread* createKernelThread();

      enum State
      {
         New,
         Idle,
         Ready,
         Running,
         Agony,
         Dead
      };

   private:   
      unsigned long idM;
//   uintptr_t userStack;
      uintptr_t kernelStackM;
      State stateM;
      Type typeM;
      Process* processM;
      Thread* nextM;
      std::string nameM;

      static unsigned long nextThreadId;

      friend void ::Kernel::Scheduler::insert(Thread* t);
      friend void ::Kernel::Scheduler::schedule();
   };
};

#endif
