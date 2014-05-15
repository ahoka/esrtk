#ifndef _PCI_H_
#define _PCI_H_

#include <stdint.h>

class PciDriver;

class Pci
{
public:
   static void init();
   static void listDevices();
   static bool probeAndAttach(uint8_t bus, uint8_t device, uint8_t function);
   static void registerDriver(PciDriver* driver);
   static void probeDevices();

   static uint8_t readConfigurationRegister8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
   static uint16_t readConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
   static uint32_t readConfigurationRegister32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

   static uint16_t getVendorId(uint8_t bus, uint8_t device, uint8_t function);
   static uint16_t getDeviceId(uint8_t bus, uint8_t device, uint8_t function);
   static const char* getClassName(uint8_t classId);

private:
   Pci();
   ~Pci();

   enum
   {
      CONFIG_REGISTER_VENDORID = 0,
      CONFIG_REGISTER_DEVICEID = 2,
   };

   static PciDriver* driversM;
};

#endif
