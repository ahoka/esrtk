#ifndef PCIDEVICE_HH
#define PCIDEVICE_HH

#include <cstdint>

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

#endif
