#ifndef THREAD_HH
#define THREAD_HH

#include <Kernel/Scheduler.hh>
#include <Kernel/Job.hh>

#include <Memory.hh>
#include <Mutex.hh>

#include <string>
#include <queue>

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

      ~Thread();

      bool addJob(const std::function<void()>&);
      void dump();

      void setName(const char*);
      std::string getName() const;

      Type getType() const;

      void setKernelStack(uintptr_t);
      uintptr_t getKernelStack() const;

      void setUserStack(uintptr_t);
      uintptr_t getUserStack() const;

      Process* getProcess() const;

      unsigned long getId() const;

      void setRunning();
      void setReady();

      static void printAll();
      static void main [[noreturn]] (Thread*);

      static Thread* createKernelThread();
      static Thread* createKernelThread(const char*);

      static Thread* createUserThread(Process*);

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

      Thread(Type);
      bool init();
      bool init0(uintptr_t stack);

      unsigned long idM;
      uintptr_t userStackM;
      uintptr_t kernelStackM;
      State stateM;
      Type typeM;
      Process* processM;
      Thread* nextM;
      std::string nameM;

      // stats
      unsigned long onCpuM;

      mutable Mutex lockM;
      std::queue<Job> jobsM;

      static unsigned long nextThreadId;

      friend class ::Kernel::Scheduler;
   };
};

#endif
