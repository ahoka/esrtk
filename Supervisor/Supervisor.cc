/* 
 * Main class for the supervisor
 */

#include "Supervisor.hh"
#include <Debug.hh>
#include <Assembly.hh>
#include <Pci.hh>
#include <System.hh>
#include <IoPort.hh>
#include <Power.hh>
#include <Apic.hh>
#include <Acpi.hh>
#include <Multiboot.hh>
#include <stdio.h>

Supervisor::Supervisor()
{
}

Supervisor::Supervisor(const Supervisor& /*orig*/)
{
}

Supervisor::~Supervisor()
{
}

extern Multiboot* mbd;
extern uint32_t magic;

extern uint32_t flags;
extern uint32_t magic1;

void
Supervisor::run()
{
   printf("========================\n");
   printf("      kernal pls        \n");
   printf("========================\n");
   
   mbd->print();

//   return;

   char id[13];
   cpuid0(id);

   printf("CPU Vendor ID: %s\n", id);
   
   // unsigned long long ulonglong = 123456789123456789ull;
   // printf("%llu\n", ulonglong / 2);
   // printf("%llu\n", ulonglong);
   
   Pci::init();
//   Pci::listDevices();

   // if (!apic.probe())
   // {
   //    printf("x2APIC not present\n");
   // }
   
   apic.printInformation();

   // printf("LAPIC ID: %u\n", apic.getLocalApicId());

   // printf("looking for RSDP:\n");

   Rsdt* rsdt = Acpi::findRsdt();
   printf("Found it at: 0x%x\n", (void*)rsdt);

   // printf("rev: %hhu, len: %u, rsdt: %p\n", rsdt->revision,
   // 	  rsdt->length, rsdt->rsdtAddress);
   
   // printf("OEM ID: %c%c%c%c%c%c\n", rsdt->oemId[0], rsdt->oemId[1],
   // 	  rsdt->oemId[2], rsdt->oemId[3], rsdt->oemId[4],
   // 	  rsdt->oemId[5]);

   // printf("done\n");

   Power::halt();
}
