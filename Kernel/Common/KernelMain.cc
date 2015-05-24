#include <Memory.hh>
#include <Modules.hh>

//#include <X86/LocalApic.hh>
#include <X86/PageDirectory.hh>
#include <X86/Idt.hh>
#include <X86/Gdt.hh>
#include <X86/Tss.hh>
#include <X86/EarlySerial.hh>
#include <Kernel/ProcessContext.hh>

#include <MemoryManager.hh>
#include <Power.hh>
#include <cstdio>

extern unsigned int magic;

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
   MemoryManager::init();

   printf("Memory management initialized...\n");
}
