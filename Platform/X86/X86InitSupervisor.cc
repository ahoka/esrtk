#include <X86/Apic.hh>
#include <X86/PageDirectory.hh>
#include <X86/Idt.hh>
#include <X86/Gdt.hh>
#include <X86/Memory.hh>
#include <Supervisor.hh>

// the kernel drivers should be able to function
// after calling this
//
// constructors will be called after this
//
void
initSupervisor()
{
   initGdt();
   initInterrupts();
   Memory::init();
   PageDirectory::init();

   apic.init();
}
