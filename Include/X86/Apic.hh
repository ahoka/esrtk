#ifndef APIC_HH
#define APIC_HH

#include <stdint.h>

class Apic
{
public:
   void init();
   void printInformation();
   int probe();

   void read32(uint32_t offset, uint32_t *a);
   void read64(uint32_t offset, uint32_t *a, uint32_t *b);
   void write32(uint32_t offset, uint32_t a);
   void write64(uint32_t offset, uint32_t a, uint32_t b);

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

   uint32_t createLocalVectorTable(LvtMask mask,
                                   LvtTriggerMode triggerMode,
                                   LvtPinPolarity polarity,
                                   LvtDeliveryMode deliveryMode,
                                   uint8_t vector);

   uint32_t createLocalVectorTable(LvtMask mask,
                                   LvtDeliveryMode deliveryMode,
                                   uint8_t vector);

private:
   uint32_t apicAddress;
   bool isBsp;
   bool isEnabled;
};

extern Apic apic;

#endif
