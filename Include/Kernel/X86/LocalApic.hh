#ifndef LOCALAPIC_HH
#define LOCALAPIC_HH

#include <stdint.h>

class LocalApic
{
public:
   LocalApic(uintptr_t physicalAddress);
   LocalApic(uintptr_t physicalAddress, uint32_t id);

   void printInformation();
   int probe();

   void endOfInterrupt();

   uint32_t read32(uint32_t offset);
   uint64_t read64(uint32_t offset);
   void write32(uint32_t offset, uint32_t value);
   void write64(uint32_t offset, uint64_t value);

   enum
   {
      IA32_APIC_BASE = 0x001b,
      IA32_APIC_BASE_BSP = (1 << 8),
      IA32_APIC_BASE_EN = (1 << 11),
      IA32_APIC_BASE_ADDRESS_MASK = 0xfffff000
   };

   enum
   {
      LocalApicId = 0x0020,
      LocalApicVersion = 0x0030,
      Eoi = 0x00b0,
      SpuriousInterruptVector = 0x0f0,
      ErrorStatus = 0x0280,
      LvtCmci = 0x02f0,
      InterruptCommandLow = 0x0300,
      InterruptCommandHigh = 0x0310,
      LvtLint0 = 0x0350,
      LvtLint1 = 0x0360,
      LvtError = 0x0370
   };

   enum
   {
      SpuriousVectorMask = 0xff,
      ApicSoftwareEnable = (1 << 8),
      ApicFocusDisabled = (1 << 9)
   };

   struct Lvt
   {
      enum DeliveryMode
      {
         DeliveryModeFixed = 0,
         DeliveryModeSmi = 2,
         DeliveryModeNmi = 4,
         DeliveryModeInit = 5,
         DeliveryModeExtInt = 7
      };

      enum DeliverytStatus
      {
         DeliveryStatusIdle = 0,
         DeliveryStatusSendPending = 1
      };

      enum Mask
      {
         NotMasked = 0,
         Masked = 1
      };

      enum TriggerMode
      {
         Edge = 0,
         Level = 1
      };

      enum PinPolarity
      {
         ActiveHigh = 0,
         ActiveLow = 1
      };
   };

   enum ApicFlags
   {
      ApicIsEnabled = (1 << 0),
      ApicIsBsp = (1 << 1)
   };

   enum
   {
      InternalApicMinVersion = 0x10
   };

   struct Icr
   {
      enum DeliveryMode
      {
         Fixed = 0x0 << 8,
         LowestPriority = 0x1 << 8,
         SMI = 0x2 << 8,
         Reserved_0 = 0x3 << 8,
         NMI = 0x4 << 8,
         INIT = 0x5 << 8,
         StartUp = 0x6 << 8,
         Reserved_1 = 0x7 << 8
      };

      enum DestinationMode
      {
         Physical = 0,
         Logical = 1 << 11
      };

      enum DeliveryStatus
      {
         Idle = 0,
         SendPending = 1 << 12
      };

      enum Level
      {
         DeAssert = 0,
         Assert = 1 << 14
      };

      enum TriggerMode
      {
         Edge = 0,
         Level = 1 << 15
      };

      enum DestinationShorthand
      {
         NoShorthand = 0x0 << 18,
         Self = 0x1 << 18,
         AllIncludingSelf = 0x2 << 18,
         AllExcludingSelf = 0x3 << 18
      };

      enum
      {
         DestinationFieldShift = 56
      };
   };

   uint32_t getLocalApicId();
   int initOtherProcessors(uintptr_t vector);

private:
   uint32_t createLocalVectorTable(Lvt::Mask mask,
                                   Lvt::TriggerMode triggerMode,
                                   Lvt::PinPolarity polarity,
                                   Lvt::DeliveryMode deliveryMode,
                                   uint8_t vector);

   uint32_t createLocalVectorTable(Lvt::DeliveryMode deliveryMode,
                                   uint8_t vector);

   void printLocalVectorTable(uint32_t lvt);

   void init();

   uintptr_t apicAddressM;
   uintptr_t physicalAddressM;

   uint32_t idM;
   uint32_t flagsM;

   bool enabledM;
};

#endif
