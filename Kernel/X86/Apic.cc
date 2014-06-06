#include <X86/Apic.hh>

#include <Debug.hh>
#include <Memory.hh>

#include <X86/Processor.hh>
#include <X86/CpuRegisters.hh>
#include <X86/IoPort.hh>

#include <cstdio>

Apic apic;

Apic::Apic()
{
   printf("Initializing APIC.\n");
   init();
}

uint32_t
Apic::read32(uint32_t offset)
{
   return *(volatile uint32_t *)(apicAddress + offset);
}

uint64_t
Apic::read64(uint32_t offset)
{
   uint32_t a, b;

   a = *(volatile uint32_t *)(apicAddress + offset);
   b = *(volatile uint32_t *)(apicAddress + offset + 4);

   return (uint64_t )a | ((uint64_t )b << 32);
}

void
Apic::write32(uint32_t offset, uint32_t value)
{
   *(volatile uint32_t *)(apicAddress + offset) = value;
}

void
Apic::write64(uint32_t offset, uint64_t value)
{
   *(volatile uint32_t *)(apicAddress + offset) = value;
   *(volatile uint32_t *)(apicAddress + offset + 4) = value >> 32;
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
   cpuid_t id;

   cpuid(0x1, &id);

   if (id.eax & CPUID::APIC)
   {
      printf("APIC: found controller\n");
   }
   else
   {
      printf("APIC: not found\n");
      enabled = false;
      return;
   }

   uint64_t msr = x86_rdmsr(IA32_APIC_BASE);
   
   apicAddress = msr & IA32_APIC_BASE_ADDRESS_MASK;

   if (msr & IA32_APIC_BASE_BSP)
   {
      flags |= ApicIsBsp;
   }
   if (msr & IA32_APIC_BASE_EN)
   {
      flags |= ApicIsEnabled;
   }

   printf("APIC: base address: 0x%x\n", apicAddress);

   apicAddress = Memory::mapPage(apicAddress);
   KASSERT(apicAddress != 0);

   printf("APIC: CPU type: %s\n",
          (flags & ApicIsBsp) ? "BSP" : "APU");
   printf("APIC: state: %s\n",
          (flags & ApicIsEnabled) ? "enabled" : "disabled");
   printf("APIC: LAPIC Id: %u\n", getLocalApicId());

   uint64_t version = read32(LocalApicVersion);
   printf("APIC: Version: 0x%hhx, LVTs: %u\n",
	  (uint8_t)(version & 0xff),
	  (uint32_t)((version >> 16) & 0x7f) + 1);

   // virtual wire mode?
   // outb(0x22, 0x70);
   // outb(0x23, 0x1);

   auto lint0 = read32(LvtLint0);
   printf("LINT0: 0x%x\n", lint0);
   printLocalVectorTable(lint0);

   auto lint1 = read32(LvtLint1);
   printf("LINT1: 0x%x\n", lint1);
   printLocalVectorTable(lint1);

//   write32(LvtLint0, createLocalVectorTable(DeliveryModeExtInt, 0));

   // XXX this should come from acpi
   write32(LvtLint1, createLocalVectorTable(DeliveryModeNmi, 0));

   uint32_t siv = read32(SpuriousInterruptVector);
   printf("APIC: Spurious Vector: %u\n", siv & SpuriousVectorMask);
   write32(SpuriousInterruptVector, siv | ApicSoftwareEnable | ApicFocusDisabled);

   enabled = true;

   // EOI
   write32(Eoi, 0x00);
}

void Apic::endOfInterrupt()
{
   // non specific now
   if (enabled)
   {
      write32(Eoi, 0x00);
   }
}

void
Apic::printInformation()
{
}

uint32_t
Apic::getLocalApicId()
{
   uint32_t apicId = apic.read32(0x20);

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

// ISA-like
uint32_t
Apic::createLocalVectorTable(LvtDeliveryMode deliveryMode,
                             uint8_t vector)
{
   return createLocalVectorTable(NotMasked, Edge, ActiveHigh,
                                 deliveryMode, vector);
}


void
Apic::printLocalVectorTable(uint32_t lvt)
{
   uint8_t mask = (lvt >> 15) & 0x1u;

   uint8_t triggerMode = (lvt >> 14) & 0x1u;
   uint8_t polarity = (lvt >> 12) & 0x1u;
   uint8_t deliveryMode = (lvt >> 7) & 0x7u;
   uint8_t vector = lvt & 0xff;

   printf("LVT: Vector: %u, Deliv: %u, Pol: %u, TMode: %u, Masked: %u\n",
          vector, deliveryMode, polarity, triggerMode, mask);
}
