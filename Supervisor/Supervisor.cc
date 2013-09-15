/* 
 * Main class for the supervisor
 */

#include <X86/Apic.hh>
#include <X86/Acpi.hh>
#include <X86/Assembly.hh>
#include <X86/IoPort.hh>
#include <X86/PageDirectory.hh>

#include <Supervisor.hh>
#include <Debug.hh>
#include <Power.hh>
#include <Multiboot.hh>
#include <Pci.hh>
#include <System.hh>

#include <cstdio>

extern Multiboot* mbd;
extern uint32_t magic;

extern uint32_t flags;
extern uint32_t magic1;

extern void* page_directory;
extern unsigned int page_directory_size;

extern "C" void
supervisor_init()
{
   Supervisor::init();
}

void
Supervisor::run()
{
   char id[13];
   cpuid0(id);

   printf("CPU Vendor ID: %s\n", id);

   Pci::init();
   Pci::listDevices();

   if (!apic.probe())
   {
      printf("x2APIC not present\n");
   }

   apic.printInformation();

   printf("LAPIC ID: %u\n", apic.getLocalApicId());

   Acpi::printAllDescriptors();

   Power::halt();
}
