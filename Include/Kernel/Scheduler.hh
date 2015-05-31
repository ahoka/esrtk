#ifndef SCHEDULER_HH
#define SCHEDULER_HH

namespace Kernel
{
   class Thread;
   class Process;

   namespace Scheduler
   {
      void init();

      void insert(Thread* t);
      void schedule();

      Thread* getCurrentThread();
      void setCurrentThread(Thread* thread);

      Process* getCurrentProcess();
      void setCurrentProcess(Process*);

      Process* getKernelProcess();
      
      void printAll();
   };
};

#endif

