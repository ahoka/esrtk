#include <Supervisor/Supervisor.hh>
#include <Kernel/Thread.hh>
#include <Kernel/Process.hh>
#include <CompilerSupport.hh>
#include <Kernel/Scheduler.hh>
#include <Supervisor/Monitor.hh>
#include <Kernel/Cpu.hh>

#include <Debug.hh>
#include <Power.hh>
#include <Mutex.hh>

#include <functional>

#include <cstdio>
#include <cstdlib>

#define TEST

namespace
{
   void supervisor_main(void *arg)
   {
      auto supervisor = (Supervisor::Supervisor *)arg;
      supervisor->run();
   }

   void monitor_main(void *arg)
   {
      auto monitor = (Monitor *)arg;
      monitor->enter();
   }
}

namespace Supervisor
{

void
Supervisor::init()
{
   printf("---> Supervisor starting\n");

   static Supervisor supervisor;
   Kernel::Thread* supervisorThread = Kernel::Thread::createKernelThread("Supervisor");
   supervisorThread->addJob(Kernel::Job(supervisor_main, &supervisor));

   static Monitor monitor;
   Kernel::Thread* monitorThread = Kernel::Thread::createKernelThread("Monitor");
   monitorThread->addJob(Kernel::Job(monitor_main, &monitor));
}

void
Supervisor::run()
{
   printf("Supervisor started\n");

#ifdef TEST
   // char* a = new char[256];
   // char* b = new char[256];
   // char* c = new char[256];
   // char* d = new char[256];

   // delete[] a;
   // delete[] b;
   // delete[] c;
   // delete[] d;

   // char* e = new char[128];
   // char* f = new char[256];
   // char* g = new char[768];

   // delete[] e;
   // delete[] f;
   // delete[] g;

//   Mutex lock;

//   lock.tryEnter();
//   lock.enter();

   Kernel::Thread::createKernelThread();
   Kernel::Thread::createKernelThread();
   Kernel::Thread::createKernelThread();

   Kernel::Process* p = Kernel::Process::createProcess();
   p->createThread();

   unsigned int seed = (unsigned int)Cpu::getTimestamp();
   printf("seeding prng: 0x%x\n", seed);
   srand(seed);

   printf("rand: %d\n", rand());
   printf("rand: %d\n", rand());
   printf("rand: %d\n", rand());
   printf("rand: %d\n", rand());

//   lock.exit();

#endif
}

}
