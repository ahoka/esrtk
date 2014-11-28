#include <X86/LocalApic.hh>

#include <Debug.hh>
#include <Memory.hh>

#include <X86/Processor.hh>
#include <X86/CpuRegisters.hh>
#include <X86/IoPort.hh>

#include <cstdio>

LocalApic::LocalApic(uintptr_t physicalAddress)
   : physicalAddressM(physicalAddress)
{
   init();

   idM = (read32(0x20) >> 24) & 0xf;

   printf("Local APIC at %p, Id: %u\n", (void*)physicalAddressM, idM);
}

LocalApic::LocalApic(uintptr_t physicalAddress, uint32_t id)
   : physicalAddressM(physicalAddress),
     idM(id)
{
   printf("Local APIC at %p, Id: %u\n", (void*)physicalAddressM, idM);
}

uint32_t
LocalApic::read32(uint32_t offset)
{
   return *(volatile uint32_t *)(apicAddressM + offset);
}

uint64_t
LocalApic::read64(uint32_t offset)
{
   uint32_t a, b;

   a = *(volatile uint32_t *)(apicAddressM + offset);
   b = *(volatile uint32_t *)(apicAddressM + offset + 4);

   return (uint64_t )a | ((uint64_t )b << 32);
}

void
LocalApic::write32(uint32_t offset, uint32_t value)
{
   *(volatile uint32_t *)(apicAddressM + offset) = value;
}

void
LocalApic::write64(uint32_t offset, uint64_t value)
{
   *(volatile uint32_t *)(apicAddressM + offset) = value;
   *(volatile uint32_t *)(apicAddressM + offset + 4) = value >> 32;
}

int
LocalApic::probe()
{
   struct cpuid id;

   // probe
   cpuid(1, &id);
   if ((id.ecx & (1 << 21)) == 0)
   {
      return 0;
   }

   return 1;
}

void
LocalApic::init()
{
   struct cpuid id;

   cpuid(0x1, &id);

   if (id.eax & CPUID::APIC)
   {
      printf("APIC: found controller\n");
   }
   else
   {
      printf("APIC: not found\n");
      enabledM = false;
      return;
   }

   uint64_t msr = x86_rdmsr(IA32_APIC_BASE);
   
   apicAddressM = msr & IA32_APIC_BASE_ADDRESS_MASK;

   if (msr & IA32_APIC_BASE_BSP)
   {
      flagsM |= ApicIsBsp;
   }

   if (msr & IA32_APIC_BASE_EN)
   {
      flagsM |= ApicIsEnabled;
   }
   else
   {
      Debug::panic("APIC is disabled\n");
   }

   printf("APIC: base address: 0x%lx\n", apicAddressM);

   apicAddressM = Memory::mapRegion(apicAddressM, 4096u, Memory::MapUncacheable);
   KASSERT(apicAddressM != 0);

   printf("APIC: CPU type: %s\n",
          (flagsM & ApicIsBsp) ? "BSP" : "APU");
   printf("APIC: state: %s\n",
          (flagsM & ApicIsEnabled) ? "enabled" : "disabled");
   printf("APIC: LAPIC Id: %u\n", getLocalApicId());

   uint64_t version = read32(LocalApicVersion);
   printf("APIC: Version: 0x%hhx, LVTs: %u\n",
	  (uint8_t)(version & 0xff),
	  (uint32_t)((version >> 16) & 0x7f) + 1);

   // virtual wire mode?
   // outb(0x22, 0x70);
   // outb(0x23, 0x1);

   auto tpr = read32(0x80);
   printf("TPR: 0x%x\n", tpr);
   write32(0x80, 0);

   auto lint0 = read32(LvtLint0);
   printf("LINT0: 0x%x\n", lint0);
   printLocalVectorTable(lint0);

   auto lint1 = read32(LvtLint1);
   printf("LINT1: 0x%x\n", lint1);
   printLocalVectorTable(lint1);

   auto perfmon = read32(LvtPerfMon);
   printf("PMON: 0x%x\n", perfmon);
   printLocalVectorTable(perfmon);

   // enable perfmon interrupt as nmi
   perfmon = createLocalVectorTable(Lvt::NotMasked, Lvt::DeliveryModeNmi, 0);
   write32(LvtPerfMon, perfmon);

   perfmon = read32(LvtPerfMon);
   printf("PMON: 0x%x\n", perfmon);
   printLocalVectorTable(perfmon);

   uint32_t siv = read32(SpuriousInterruptVector);
   printf("APIC: Spurious Vector: %u\n", siv & SpuriousVectorMask);
   printf("APIC: Spurious Vector Register: 0x%x\n", siv);

   siv = 255 | ApicSoftwareEnable | ApicFocusDisabled;
   write32(SpuriousInterruptVector, siv);
   siv = read32(SpuriousInterruptVector);
   printf("APIC: Spurious Vector Register: 0x%x\n", siv);

   enabledM = true;

   // EOI
   write32(Eoi, 0x00);

   // timer
   
}

#if 0
void
LocalApic::setLocalInt(int pin)
{
//   write32(LvtLint0, createLocalVectorTable(DeliveryModeExtInt, 0));
   
   // XXX this should come from acpi
   write32(LvtLint1, createLocalVectorTable(Lvt::DeliveryModeNmi, 0));
}
#endif

void
LocalApic::endOfInterrupt()
{
   // non specific now
   if (enabledM)
   {
      write32(Eoi, 0x00);
   }
}

int
LocalApic::initOtherProcessors(uintptr_t vector)
{
   assert(vector < 0x100000);

   return 0;
}

void
LocalApic::printInformation()
{
}

uint32_t
LocalApic::getLocalApicId()
{
   uint32_t apicId = read32(0x20);

   // XXX
   return (apicId >> 24) & 0xf;
}

uint32_t
LocalApic::createLocalVectorTable(Lvt::Mask mask,
                             Lvt::TriggerMode triggerMode,
                             Lvt::PinPolarity polarity,
                             Lvt::DeliveryMode deliveryMode,
                             uint8_t vector)
{
   uint32_t lvt = 0;

   lvt |= (mask & 0x1u) << Lvt::MaskShift;
   lvt |= (triggerMode & 0x1u) << Lvt::TriggerModeShift;
   lvt |= (polarity & 0x1u) << Lvt::PolarityShift;
   lvt |= (deliveryMode & 0x7u) << Lvt::DeliveryModeShift;
   lvt |= vector;

   return lvt;
}

uint32_t
LocalApic::createLocalVectorTable(Lvt::Mask mask,
                                  Lvt::DeliveryMode deliveryMode,
                                  uint8_t vector)
{
   uint32_t lvt = 0;

   lvt |= (mask & 0x1u) << 16;
   lvt |= (deliveryMode & 0x7u) << 8;
   lvt |= vector;

   return lvt;
}

// ISA-like
uint32_t
LocalApic::createLocalVectorTable(Lvt::DeliveryMode deliveryMode,
                             uint8_t vector)
{
   return createLocalVectorTable(Lvt::NotMasked, Lvt::Edge, Lvt::ActiveHigh,
                                 deliveryMode, vector);
}

void
LocalApic::printLocalVectorTable(uint32_t lvt)
{
   uint8_t mask = (lvt >> 16) & 0x1u;

   uint8_t triggerMode = (lvt >> 15) & 0x1u;
   uint8_t polarity = (lvt >> 13) & 0x1u;
   uint8_t deliveryMode = (lvt >> 8) & 0x7u;
   uint8_t vector = lvt & 0xff;

   printf("LVT: Vector: %u, Deliv: %u, Pol: %u, TMode: %u, Masked: %u\n",
          vector, deliveryMode, polarity, triggerMode, mask);
}
