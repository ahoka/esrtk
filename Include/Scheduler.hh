#ifndef SCHEDULER_HH
#define SCHEDULER_HH

#include <KernelThread.hh>
#include <DoublyLinkedList.hh>

class Scheduler
{
public:
   Scheduler();
   ~Scheduler();

   static void tick();
   static void dispatch();

private:
//   static KernelThread threads;
};

#endif

