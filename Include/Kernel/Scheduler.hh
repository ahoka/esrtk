#ifndef SCHEDULER_HH
#define SCHEDULER_HH

namespace Kernel
{
   class Thread;

   namespace Scheduler
   {
      void init();

      void insert(Thread* t);
      void schedule();

      Thread* getCurrentThread();
      void setCurrentThread(Thread* thread);

      void printAll();
   };
};

#endif

