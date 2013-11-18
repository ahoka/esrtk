#include <Memory.hh>
#include <Supervisor.hh>
#include <Modules.hh>

#include <X86/Apic.hh>
#include <X86/PageDirectory.hh>
#include <X86/Idt.hh>
#include <X86/Gdt.hh>
#include <X86/SerialConsole.hh>

// the kernel drivers should be able to function
// after calling this
//
// constructors will be called after this
//
void
Supervisor::init()
{
   gdt_init();
   SerialConsole::init();
   initInterrupts();
   Memory::copyMemoryMap();
   Modules::init(); // mi called from md, not good!
   PageDirectory::init();
   Memory::init();
}
