#include <Pci.hh>
#include <Driver/PciDriver.hh>
#include <cstdio>

static PciDriver* pciDrivers = 0;

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

void
Pci::getDeviceInfo(PciDevice pciDevice)
{
   uint8_t interruptPin = pciDevice.readConfigurationRegister8(Pci::Config::InterruptPin);
   uint8_t interruptLine = pciDevice.readConfigurationRegister8(Pci::Config::InterruptLine);

   printf("int: %u %u\n", interruptPin, interruptLine);

   const char* pins[] = {
      "None",
      "INTA#",
      "INTB#",
      "INTC#",
      "INTD#",
   };

   if (interruptPin != 0 && interruptPin <= 4)
   {
      printf("IRQ%u (%s)\n", interruptLine, pins[interruptPin]);
   }
}

uint16_t
Pci::getVendorId(uint8_t bus, uint8_t device, uint8_t function)
{
   return readConfigurationRegister16(bus, device, function, Config::VendorId);
}

uint16_t
Pci::getDeviceId(uint8_t bus, uint8_t device, uint8_t function)
{
   return readConfigurationRegister16(bus, device, function, Config::DeviceId);
}

const char*
Pci::getClassName(uint8_t classId)
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

void
Pci::enumerate()
{
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
               getDeviceInfo(PciDevice(bus, dev, fun));
               if (!probeAndAttach(bus, dev, fun))
               {
                  printf("pci%x:%x:%x: Unknown %s: 0x%0hx:0x%0hx\n",
                         bus, dev, fun, getClassName(deviceClass), vid, did);
               }
	    }
	 }
      }
   }
}

void
Pci::registerDriver(PciDriver* driver)
{
   printf("DriverManager: registering driver %p\n", driver);

   driver->next = pciDrivers;
   pciDrivers = driver;
}

bool
Pci::probeAndAttach(uint8_t bus, uint8_t device, uint8_t function)
{
   PciDriver* driver = pciDrivers;
   while (driver != 0)
   {
      // TODO attach 'best'
      if (driver->probe(bus, device, function) > 0)
      {
         printf("pci%x:%x:%x: Initializing driver %s\n", bus, device, function, driver->name());
         driver->init(bus, device, function);

         return true;
      }

      driver = driver->next;
   }

   return false;
}
