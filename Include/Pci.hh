#ifndef _PCI_H_
#define _PCI_H_

#include <stdint.h>

class PciDriver;

class PciDevice
{
public:
   PciDevice(uint8_t bus, uint8_t device, uint8_t function);
   PciDevice(const PciDevice& orig);
   PciDevice& operator=(const PciDevice& orig);

   uint8_t readConfigurationRegister8(uint8_t offset);
   uint16_t readConfigurationRegister16(uint8_t offset);
   uint32_t readConfigurationRegister32(uint8_t offset);

   void writeConfigurationRegister8(uint8_t offset, uint8_t value);
   void writeConfigurationRegister16(uint8_t offset, uint16_t value);
   void writeConfigurationRegister32(uint8_t offset, uint32_t value);

private:
   uint8_t busM;
   uint8_t deviceM;
   uint8_t functionM;
};

namespace Pci
{
   void init();
   void listDevices();
   bool probeAndAttach(uint8_t bus, uint8_t device, uint8_t function);
   void registerDriver(PciDriver* driver);
   void enumerate();

   void getDeviceInfo(PciDevice pciDevice);

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
         Status = 0x06,
         Command = 0x04,
         Bar0 = 0x10,
         Bar1 = 0x14,
         Bar2 = 0x18,
         Bar3 = 0x1c,
         Bar4 = 0x20,
         Bar5 = 0x24,
         InterruptPin = 0x3d,
         InterruptLine = 0x3c
      };
   };
};

#endif
