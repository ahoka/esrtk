#include <Kernel/PciDevice.hh>
#include <Kernel/Pci.hh>

PciDevice::PciDevice(uint8_t bus, uint8_t device, uint8_t function)
   : busM(bus),
     deviceM(device),
     functionM(function)
{
}

PciDevice::PciDevice(const PciDevice& orig)
   : busM(orig.busM),
     deviceM(orig.deviceM),
     functionM(orig.functionM)
{
}

PciDevice&
PciDevice::operator=(const PciDevice& orig)
{
   if (this != &orig)
   {
      busM = orig.busM;
      deviceM = orig.deviceM;
      functionM = orig.functionM;
   }

   return *this;
}

uint8_t
PciDevice::readConfigurationRegister8(uint8_t offset)
{
   return Pci::readConfigurationRegister8(busM, deviceM, functionM, offset);
}

uint16_t
PciDevice::readConfigurationRegister16(uint8_t offset)
{
   return Pci::readConfigurationRegister16(busM, deviceM, functionM, offset);
}

uint32_t
PciDevice::readConfigurationRegister32(uint8_t offset)
{
   return Pci::readConfigurationRegister32(busM, deviceM, functionM, offset);
}

void
PciDevice::writeConfigurationRegister8(uint8_t offset, uint8_t value)
{
   Pci::writeConfigurationRegister8(busM, deviceM, functionM, offset, value);
}

void
PciDevice::writeConfigurationRegister16(uint8_t offset, uint16_t value)
{
   Pci::writeConfigurationRegister16(busM, deviceM, functionM, offset, value);
}

void
PciDevice::writeConfigurationRegister32(uint8_t offset, uint32_t value)
{
   Pci::writeConfigurationRegister32(busM, deviceM, functionM, offset, value);
}
