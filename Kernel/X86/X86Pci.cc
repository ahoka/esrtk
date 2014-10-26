#include <X86/IoPort.hh>
#include <Pci.hh>
#include <Debug.hh>
#include <cstdio>

class X86Pci
{
public:
   static const int PCI_CONFIG_ADDRESS = 0x0cf8;
   static const int PCI_CONFIG_DATA = 0x0cfc;
   enum
   {
      PCI_MODE1_ENABLE =	 0x80000000UL,
      PCI_MODE1_ADDRESS_REG =	 0x0cf8,
      PCI_MODE1_DATA_REG =	 0x0cfc
   };

   static uint32_t makeTag(uint8_t bus, uint8_t device, uint8_t function)
   {
      return (1u << 31) | ((uint32_t )bus << 16) |
	 ((uint32_t )device << 11) | ((uint32_t )function << 8);
   }
};

uint8_t
Pci::readConfigurationRegister8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
   uint32_t value = readConfigurationRegister32(bus, device, function, offset);
   int suboff = (offset & 0x03) * 8;

   return (uint8_t )(value >> suboff);
}

uint16_t
Pci::readConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
   uint32_t value = readConfigurationRegister32(bus, device, function, offset);
   int suboff = ((offset & 0x02) >> 1) * 16;

   return (uint16_t )(value >> suboff);
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

void
Pci::writeConfigurationRegister8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t val)
{
   uint32_t value = readConfigurationRegister32(bus, device, function, offset);
   int suboff = (offset & 0x03) * 8;

   value &= (0xff >> suboff);
   value |= (val >> suboff);

   return writeConfigurationRegister32(bus, device, function, offset & (~0x03), value);
}

void
Pci::writeConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint16_t val)
{
   uint32_t value = readConfigurationRegister32(bus, device, function, offset);
   int suboff = ((offset & 0x02) >> 1) * 16;

   value &= (0xffff >> suboff);
   value |= (val >> suboff);

   return writeConfigurationRegister32(bus, device, function, offset & (~0x03), value);
}

// 31         |  30 - 24 | 23 - 16    | 15 - 11       | 10 - 8          | 7 - 2           | 1 - 0
// Enable Bit | Reserved | Bus Number | Device Number | Function Number | Register Number | 00

void
Pci::writeConfigurationRegister32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t val)
{
   uint32_t tag = X86Pci::makeTag(bus, device, function) | (offset & 0xfc);

   outl(X86Pci::PCI_CONFIG_ADDRESS, tag);

   return outl(X86Pci::PCI_CONFIG_DATA, val);
}

void
Pci::init()
{
   outl(X86Pci::PCI_MODE1_ADDRESS_REG, X86Pci::PCI_MODE1_ENABLE);
   outb(X86Pci::PCI_MODE1_ADDRESS_REG + 3, 0);
   outw(X86Pci::PCI_MODE1_ADDRESS_REG + 2, 0);

   uint32_t val = inl(X86Pci::PCI_MODE1_ADDRESS_REG);
   if ((val & 0x80fffffc) != X86Pci::PCI_MODE1_ENABLE) {
      // sometimes happen on qemu but still works?
      printf("PCI init FAILED! Not MODE1 PCI?\n");
      return;
   }
   else
   {
      printf("PCI init OK.\n");
   }
}
