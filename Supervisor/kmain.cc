#include <MemoryManager.hh>
#include <Supervisor.hh>
#include <CompilerSupport.hh>
#include <Power.hh>
#include <cstdio>

#include <X86/Acpi.hh>

extern "C" void kmain(void) __attribute__((noreturn));

void
kmain(void)
{
   extern unsigned int magic;

   if (magic != 0x2badb002)
   {
      printf("Incorrect multiboot magic: 0x%0x != 0x%0x\n", magic, 0x2badb002);
      // not loaded by a multiboot loader, this is not yet supported
      //
      Power::halt();
   }

   MemoryManager::init();

   printf("Supervisor starting\n");

   // Call C++ constructors
   __cxaimpl_call_constructors();

   Supervisor::run();
}
