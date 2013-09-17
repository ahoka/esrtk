#include <X86/IoPort.hh>
#include <Pci.hh>
#include <Debug.hh>
#include <cstdio>

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

   return (uint8_t )(inl(X86Pci::PCI_CONFIG_DATA) >> suboff);
}

uint16_t
Pci::readConfigurationRegister16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset)
{
   uint32_t tag = X86Pci::makeTag(bus, device, function) | (offset & 0xfc);
   int suboff = ((offset & 0x02) >> 1) * 16;

   outl(X86Pci::PCI_CONFIG_ADDRESS, tag);

   return (uint16_t )inl(X86Pci::PCI_CONFIG_DATA) >> suboff;
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
Pci::init()
{
   outl(PCI_MODE1_ADDRESS_REG, PCI_MODE1_ENABLE);
   outb(PCI_MODE1_ADDRESS_REG + 3, 0);
   outw(PCI_MODE1_ADDRESS_REG + 2, 0);

   uint32_t val = inl(PCI_MODE1_ADDRESS_REG);
   if ((val & 0x80fffffc) != PCI_MODE1_ENABLE) {
      // sometimes happen on qemu but still works?
      printf("PCI init FAILED! Not MODE1 PCI?\n");
      return;
   }
   else
   {
      printf("PCI init OK.\n");
   }

   listDevices();
}

void
Pci::listDevices()
{
   printf("Listing PCI devices:\n");

   for (int bus = 0; bus < 256; bus++)
   {
      for (int dev = 0; dev < 32; dev++)
      {
	 for (int fun = 0; fun < 8; fun++)
	 {
	    uint16_t vid = Pci::getVendorId((uint8_t )bus, (uint8_t )dev, (uint8_t )fun);
	    if (vid != 0xffff)
	    {
	       uint16_t did = Pci::getDeviceId((uint8_t )bus, (uint8_t )dev, (uint8_t )fun);
               uint8_t deviceClass = readConfigurationRegister8((uint8_t )bus, (uint8_t )dev, (uint8_t )fun, 8);
	       printf("%x:%x:%x %s: 0x%0hx:0x%0hx\n", bus, dev, fun, getClassName(deviceClass), vid, did);
	    }
	 }
      }
   }
}
