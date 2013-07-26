#include <X86/Apic.hh>
#include <X86/PageDirectory.hh>
#include <X86/Idt.hh>
#include <X86/Gdt.hh>
#include <Supervisor.hh>

// the kernel drivers should be able to function
// after calling this
//
void
initSupervisor()
{
   initGdt();
   initInterrupts();
   PageDirectory::init();
   apic.init();
}
