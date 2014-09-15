#include <Supervisor/Supervisor.hh>
#include <Kernel/Thread.hh>

#include <Platform.hh>
#include <Debug.hh>
#include <Power.hh>
#include <Mutex.hh>

#include <cstdio>

#define TEST

extern "C" void
supervisor_init()
{
   Supervisor::init();
}

void
Supervisor::run()
{
   printf("Supervisor started\n");

   Platform::init();

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

   Kernel::Thread t1, t2, t3;
   t1.init();
   t2.init();
   t3.init();

//   lock.exit();

#endif

   Power::halt();
}
