#include <Memory.hh>
#include <Modules.hh>
#include <Platform.hh>
#include <CompilerSupport.hh>
#include <Kernel/Scheduler.hh>
#include <Multiboot.hh>

// XXX remove these
#include <X86/PageDirectory.hh>
#include <X86/Idt.hh>
#include <X86/EarlySerial.hh>

#include <Kernel/ProcessContext.hh>
#include <Supervisor/Supervisor.hh>

#include <Kernel/PerfCounter.hh>
#include <Kernel/Heap.hh>
#include <Power.hh>
#include <cstdio>

extern unsigned int magic;
extern "C" void mi_startup(void);

extern "C" void
kmain()
{
   if (magic != 0x2badb002)
   {
      printf("Incorrect multiboot magic: 0x%0x != 0x%0x\n", magic, 0x2badb002);
      // not loaded by a multiboot loader, this is not yet supported
      //
      Power::halt();
   }

   Kernel::ProcessContext::init();

   EarlySerial::init();

   printf("Kernel main starting...\n");

   initInterrupts();

   Memory::copyMemoryMap();
   Modules::init();

   PageDirectory::init();
   Memory::init();
   Kernel::Heap::init();

   printf("Memory management initialized...\n");

   Kernel::Scheduler::init();

   // Call C++ constructors
   __cxaimpl_call_constructors();

   Platform::init();

   Multiboot::getSymbols();

   Supervisor::Supervisor::init();

   printf("Initializing BSD...\n");
   mi_startup();

   printf("Becoming idle thread...\n");
   for (;;)
   {
      Power::halt();
   }
}
