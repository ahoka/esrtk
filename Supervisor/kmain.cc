#include <Supervisor.hh>
#include <CompilerSupport.hh>
#include <Power.hh>

extern "C" void kmain(void) __attribute__((noreturn));

void
kmain(void)
{
   extern unsigned int magic;
 
   if (magic != 0x2BADB002) {
      // not loaded by a multiboot loader, this is not yet supported
      //
      Power::halt();
   }

   // Call MD init functions
   //
   initSupervisor();

   // Call C++ constructors
   __cxaimpl_call_constructors();

   Supervisor supervisor;
   supervisor.run();
}
