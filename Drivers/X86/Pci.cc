#include <Pci.hh>
#include <stdio.hh>
#include <IoPort.hh>

class X86Pci
{
public:
   static const int PCI_CONFIG_ADDRESS = 0x0cf8;
   static const int PCI_CONFIG_DATA = 0x0cfc;

   static uint32_t makeTag(uint8_t bus, uint8_t device, uint8_t function)
   {
      return (1u << 31) | ((uint32_t )bus << 16) |
	 ((uint32_t )device << 11) | ((uint32_t )function << 8);
   }
};

uint8_t
Pci::readConfigurationRegister8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
   uint32_t tag = X86Pci::makeTag(bus, device, function) | (offset & 0xfc);
   int suboff = (offset & 0x03) * 8;

   outl(X86Pci::PCI_CONFIG_ADDRESS, tag);

   return inl(X86Pci::PCI_CONFIG_DATA) >> suboff;
}

uint16_t
Pci::readConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
   uint32_t tag = X86Pci::makeTag(bus, device, function) | (offset & 0xfc);
   int suboff = ((offset & 0x02) >> 1) * 16;

   outl(X86Pci::PCI_CONFIG_ADDRESS, tag);

   return inl(X86Pci::PCI_CONFIG_DATA) >> suboff;
}

// 31         |  30 - 24 | 23 - 16    | 15 - 11       | 10 - 8          | 7 - 2           | 1 - 0
// Enable Bit | Reserved | Bus Number | Device Number | Function Number | Register Number | 00

uint32_t
Pci::readConfigurationRegister32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
   uint32_t tag = X86Pci::makeTag(bus, device, function) | (offset & 0xfc);

   outl(X86Pci::PCI_CONFIG_ADDRESS, tag);

   return inl(X86Pci::PCI_CONFIG_DATA);
}
