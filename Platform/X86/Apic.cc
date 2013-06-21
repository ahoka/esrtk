#include <Debug.hh>
#include <stdio.h>
#include <Assembly.hh>

#include <Apic.hh>

Apic apic;

void
Apic::read32(uint32_t offset, uint32_t *a)
{
   *a = *(volatile uint32_t *)(apicAddress + offset);
}

void
Apic::read64(uint32_t offset, uint32_t *a, uint32_t *b)
{
   *a = *(volatile uint32_t *)(apicAddress + offset);
   *b = *(volatile uint32_t *)(apicAddress + offset + 4);
}

void
Apic::write32(uint32_t offset, uint32_t a)
{
   *(volatile uint32_t *)(apicAddress + offset) = a;
}

void
Apic::write64(uint32_t offset, uint32_t a, uint32_t b)
{
   *(volatile uint32_t *)(apicAddress + offset) = a;
   *(volatile uint32_t *)(apicAddress + offset + 4) = b;
}

int
Apic::probe()
{
   cpuid_t id;

   // probe
   cpuid(1, &id);
   if ((id.ecx & (1 << 21)) == 0)
   {
      return 0;
   }

   return 1;
}

void
Apic::init()
{
   uint32_t eax, edx;
   rdmsr(0x001b, &eax, &edx);

   // make sure the apic is at a 32bit address
   // KASSERT((edx & 0x0f) == 0);
   // KASSERT(false);
   
   apicAddress = eax & 0xfffff000;
   isBsp = eax & 0x100;
   isEnabled = eax & 0x800;
}

void
Apic::printInformation()
{
   printf("APIC base address: 0x%x\n", apicAddress);
   printf("APIC CPU type: %s\n", isBsp ? "BSP" : "APU");
   printf("APIC state: %s\n", isEnabled ? "enabled" : "disabled");
}

uint32_t
Apic::getLocalApicId()
{
   uint32_t apicId;
   
   apic.read32(0x20, &apicId);
   
   // XXX
   return (apicId >> 24) & 0xf;
}
