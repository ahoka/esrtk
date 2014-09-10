#ifndef SCHEDULER_HH
#define SCHEDULER_HH

class Thread;

class Scheduler
{
public:
   static void insert(Thread* t);
   static void schedule();

   static Thread* getCurrentThread();
   static void setCurrentThread(Thread* thread);

private:
   Scheduler() = delete;
   ~Scheduler() = delete;

   static Thread* threads;
   static Thread* nextToRun;
   static Thread* currentThread;
};

#endif

