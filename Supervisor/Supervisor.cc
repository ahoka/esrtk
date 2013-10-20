#include <Supervisor.hh>

#include <Platform.hh>
#include <Debug.hh>
#include <Power.hh>

#include <cstdio>

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

   char* a = new char[256];
   char* b = new char[256];
   char* c = new char[256];
   char* d = new char[256];

   delete[] a;
   delete[] b;
   delete[] c;
   delete[] d;

   char* e = new char[128];
   char* f = new char[256];
   char* g = new char[768];

   delete[] e;
   delete[] f;
   delete[] g;

   Power::halt();
}
