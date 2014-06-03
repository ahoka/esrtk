#ifndef APIC_HH
#define APIC_HH

#include <stdint.h>

class Apic
{
public:
   Apic();

   void printInformation();
   int probe();

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
      LocalApicIdRegister = 0x0020,
      LocalApicVersionRegister = 0x0030,
      EoiRegister = 0x00b0,
      ErrorStatusRegister = 0x0280
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

   // For LINT0 and LINT1
   uint32_t createLocalVectorTable(LvtMask mask,
                                   LvtTriggerMode triggerMode,
                                   LvtPinPolarity polarity,
                                   LvtDeliveryMode deliveryMode,
                                   uint8_t vector);

   uint32_t createLocalVectorTable(LvtMask mask,
                                   LvtDeliveryMode deliveryMode,
                                   uint8_t vector);

   uint32_t getLocalApicId();
private:
   void init();

   uint32_t apicAddress;
   uint32_t flags;
};

extern Apic apic;

#endif
