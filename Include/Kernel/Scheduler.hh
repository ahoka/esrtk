#ifndef SCHEDULER_HH
#define SCHEDULER_HH

namespace Kernel
{
   class Thread;
   class Process;

   class Scheduler
   {
   public:
      static void init();

      static void insert(Thread* t);
      static void remove(Thread* t);
      static void schedule();

      static Thread* getCurrentThread();
      static void setCurrentThread(Thread* thread);

      static Process* getCurrentProcess();
      static void setCurrentProcess(Process*);

      static Process* getKernelProcess();
      static Thread* getIdleThread();
      
      static void printAll();

   private:
      Scheduler() = delete;
      ~Scheduler() = delete;
      Scheduler(Scheduler&) = delete;
   };
};

#endif

