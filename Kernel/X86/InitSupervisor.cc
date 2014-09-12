#include <Memory.hh>
#include <Supervisor/Supervisor.hh>
#include <Modules.hh>
#include <Supervisor/Scheduler.hh>

//#include <X86/LocalApic.hh>
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
   x86_gdt_init();
   SerialConsole::init();
   initInterrupts();
   Memory::copyMemoryMap();
   Modules::init(); // mi called from md, not good!
   PageDirectory::init();
   Memory::init();
   Kernel::Scheduler::init();
}
