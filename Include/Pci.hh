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

   static const char*
   getClassName(uint8_t classId)
   {
      switch (classId)
      {
         case 0x00: return "Unclassified device";
         case 0x01: return "Mass storage controller";
         case 0x02: return "Network controller";
         case 0x03: return "Display controller";
         case 0x04: return "Multimedia device";
         case 0x05: return "Memory Controller";
         case 0x06: return "Bridge Device";
         case 0x07: return "Communications controller";
         case 0x08: return "Generic system peripheral";
         case 0x09: return "Input device";
         case 0x0a: return "Docking Station";
         case 0x0b: return "Processor";
         case 0x0c: return "Serial bus controller";
         case 0x0e: return "Intelligent controller";
         case 0x0f: return "Satellite communications controller";
         case 0x10: return "Encryption controller";
         case 0x11: return "Signal processing controller";
         case 0xFF: return "Misc";
         default: return "Unknown";
      }
   }
};

#endif
