#include <Debug.hh>
#include <Memory.hh>

#include <X86/Assembly.hh>
#include <X86/Apic.hh>
#include <X86/CpuRegisters.hh>

#include <cstdio>

Apic apic;

Apic::Apic()
{
   printf("Initializing APIC.\n");
   init();
}

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
   cpuid_t id;

   cpuid(0x1, &id);

   if (id.eax & CPUID::APIC)
   {
      printf("APIC: found controller\n");
   }
   else
   {
      printf("APIC: not found\n");
      return;
   }

   rdmsr(0x001b, &eax, &edx);

   // make sure the apic is at a 32bit address
   // KASSERT((edx & 0x0f) == 0);
   // KASSERT(false);
   
   apicAddress = eax & 0xfffff000;

   if (eax & 0x100)
   {
      flags |= ApicIsBsp;
   }
   if (eax & 0x800)
   {
      flags |= ApicIsEnabled;
   }

   printf("APIC: base address: 0x%x\n", apicAddress);

   apicAddress = Memory::mapPage(apicAddress);
   KASSERT(apicAddress != 0);

   printf("APIC: CPU type: %s\n", (flags & ApicIsBsp) ? "BSP" : "APU");
   printf("APIC: state: %s\n", (flags & ApicIsEnabled) ? "enabled" : "disabled");
   printf("APIC: LAPIC Id: %u\n", getLocalApicId());
}

void
Apic::printInformation()
{
}

uint32_t
Apic::getLocalApicId()
{
   uint32_t apicId;
   
   apic.read32(0x20, &apicId);
   
   // XXX
   return (apicId >> 24) & 0xf;
}

uint32_t
Apic::createLocalVectorTable(LvtMask mask,
                             LvtTriggerMode triggerMode,
                             LvtPinPolarity polarity,
                             LvtDeliveryMode deliveryMode,
                             uint8_t vector)
{
   uint32_t lvt = 0;

   lvt |= (mask & 0x1u) << 15;
   lvt |= (triggerMode & 0x1u) << 14;
   lvt |= (polarity & 0x1u) << 12;
   lvt |= (deliveryMode & 0x7u) << 7;
   lvt |= vector;

   return lvt;
}

uint32_t
Apic::createLocalVectorTable(LvtMask mask,
                             LvtDeliveryMode deliveryMode,
                             uint8_t vector)
{
   return createLocalVectorTable(mask,
                                 (LvtTriggerMode )0,
                                 (LvtPinPolarity )0,
                                 deliveryMode,
                                 vector);
}
