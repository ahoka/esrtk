#include <Memory.hh>
#include <Supervisor/Supervisor.hh>
#include <Modules.hh>
#include <Supervisor/Thread.hh>

//#include <X86/LocalApic.hh>
#include <X86/PageDirectory.hh>
#include <X86/Idt.hh>
#include <X86/Gdt.hh>
#include <X86/SerialConsole.hh>

Thread Supervisor::thread0(0);

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

   thread0.init(0, StackStart);
}
