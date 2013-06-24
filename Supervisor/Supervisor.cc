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

   Rsdp* rsdp = Acpi::findRsdp();

   if (rsdp->calculateExtendedChecksum() == 0)
   {
      //Xsdt* xsdt = (Xsdt *)rsdp->xsdtAddress;
      Rsdt* rsdt = (Rsdt *)rsdp->rsdtAddress;

      rsdt->printHeader();
      for (uint32_t i = 0;
	   i < (rsdt->length - sizeof(DescriptionHeader)) / 4;
	   i++)
      {
      	 ((DescriptionHeader *)rsdt->entry[i])->printHeader();
	 printf("\n");
      }
   }

   // printf("rev: %hhu, len: %u, rsdp: %p\n", rsdp->revision,
   // 	  rsdp->length, rsdp->rsdpAddress);
   
   // printf("OEM ID: %c%c%c%c%c%c\n", rsdp->oemId[0], rsdp->oemId[1],
   // 	  rsdp->oemId[2], rsdp->oemId[3], rsdp->oemId[4],
   // 	  rsdp->oemId[5]);

   // printf("done\n");

   Power::halt();
}
