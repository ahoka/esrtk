#ifndef _PCI_H_
#define _PCI_H_

#include <stdint.h>

class Pci
{
private:
   Pci();
   ~Pci();

   enum
   {
      CONFIG_REGISTER_VENDORID = 0,
      CONFIG_REGISTER_DEVICEID = 2,
      PCI_MODE1_ENABLE =	 0x80000000UL,
      PCI_MODE1_ADDRESS_REG =	 0x0cf8,
      PCI_MODE1_DATA_REG =	 0x0cfc
   };

public:
   static uint8_t readConfigurationRegister8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
   static uint16_t readConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
   static uint32_t readConfigurationRegister32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
   static void init();
   static void listDevices();

   static uint16_t getVendorId(uint8_t bus, uint8_t device, uint8_t function)
   {
      return readConfigurationRegister16(bus, device, function, CONFIG_REGISTER_VENDORID);
   }

   static uint16_t getDeviceId(uint8_t bus, uint8_t device, uint8_t function)
   {
      return readConfigurationRegister16(bus, device, function, CONFIG_REGISTER_DEVICEID);
   }
};

#endif
