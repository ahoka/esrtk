#ifndef APIC_HH
#define APIC_HH

#include <stdint.h>

class Apic
{
public:
   Apic();

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
   };

   enum LvtDeliveryMode
   {
      DeliveryModeFixed = 0,
      DeliveryModeSmi = 2,
      DeliveryModeNmi = 4,
      DeliveryModeInit = 5,
      DeliveryModeExtInt = 7
   };

   enum LvtDeliverytStatus
   {
      DeliveryStatusIdle = 0,
      DeliveryStatusSendPending = 1
   };

   enum LvtMask
   {
      NotMasked = 0,
      Masked = 1
   };

   enum LvtTriggerMode
   {
      Edge = 0,
      Level = 1
   };

   enum LvtPinPolarity
   {
      ActiveHigh = 0,
      ActiveLow = 1
   };

   enum ApicFlags
   {
      ApicIsEnabled = (1 << 0),
      ApicIsBsp = (1 << 1)
   };

   uint32_t createLocalVectorTable(LvtMask mask,
                                   LvtTriggerMode triggerMode,
                                   LvtPinPolarity polarity,
                                   LvtDeliveryMode deliveryMode,
                                   uint8_t vector);

   uint32_t createLocalVectorTable(LvtDeliveryMode deliveryMode,
                                   uint8_t vector);

   uint32_t getLocalApicId();
private:
   void init();

   uint32_t apicAddress;
   uint32_t flags;

   bool enabled;
};

extern Apic apic;

#endif
