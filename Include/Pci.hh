#ifndef _PCI_H_
#define _PCI_H_

#include <stdint.h>

class PciDriver;

namespace Pci
{
   void init();
   void listDevices();
   bool probeAndAttach(uint8_t bus, uint8_t device, uint8_t function);
   void registerDriver(PciDriver* driver);
   void enumerate();

   uint8_t readConfigurationRegister8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
   uint16_t readConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
   uint32_t readConfigurationRegister32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

   void writeConfigurationRegister8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t value);
   void writeConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint16_t value);
   void writeConfigurationRegister32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t value);

   uint16_t getVendorId(uint8_t bus, uint8_t device, uint8_t function);
   uint16_t getDeviceId(uint8_t bus, uint8_t device, uint8_t function);

   const char* getClassName(uint8_t classId);

   struct Config
   {
      enum
      {
         VendorId = 0x00,
         DeviceId = 0x02,
         Status = 0x04,
         Command = 0x06,
         Bar0 = 0x10,
         Bar1 = 0x14,
         Bar2 = 0x18,
         Bar3 = 0x1c,
         Bar4 = 0x20,
         Bar5 = 0x24
      };
   };
};

#endif
