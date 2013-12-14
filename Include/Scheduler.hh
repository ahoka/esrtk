#ifndef SCHEDULER_HH
#define SCHEDULER_HH

class Thread;

class Scheduler
{
public:
   Scheduler();
   ~Scheduler();

   static void insert(Thread* t);
   static void tick();
   static void dispatch();

private:
   static Thread* threads;
   static Thread* nextToRun;
};

#endif

