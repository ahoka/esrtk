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

      bool addJob(const Job&);
      void dump();

      void setName(const char*);
      std::string getName() const;

      Type getType() const;

      void setKernelStack(uintptr_t);
      uintptr_t getKernelStack() const;

      void setUserStack(uintptr_t);
      uintptr_t getUserStack() const;

      Process* getProcess() const;

      uint64_t getId() const;

      void setRunning();
      void setReady();
      bool isIdle();

      static void printAll();
      static void main [[noreturn]] (Thread*);

      static Thread* createKernelThread();
      static Thread* createKernelThread(const char*);

      static Thread* createUserThread(Process*);

      static Thread* yield();

      enum State
      {
         New,
	 Initialized,
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
      bool initIdle();

      uint64_t idM;
      uintptr_t userStackM;
      uintptr_t kernelStackM;
      volatile State stateM;
      Type typeM;
      Process* processM;
      Thread* nextM;
      std::string nameM;

      // stats
      unsigned long onCpuM;

      mutable Mutex lockM;
      std::queue<Job> jobsM;

      friend class ::Kernel::Scheduler;
   };
};

#endif
