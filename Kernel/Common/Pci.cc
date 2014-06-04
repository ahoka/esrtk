#include <Pci.hh>
#include <Driver/PciDriver.hh>
#include <cstdio>

PciDriver* Pci::driversM = 0;

uint16_t
Pci::getVendorId(uint8_t bus, uint8_t device, uint8_t function)
{
   return readConfigurationRegister16(bus, device, function, CONFIG_REGISTER_VENDORID);
}

uint16_t
Pci::getDeviceId(uint8_t bus, uint8_t device, uint8_t function)
{
   return readConfigurationRegister16(bus, device, function, CONFIG_REGISTER_DEVICEID);
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
Pci::probeDevices()
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
               if (!probeAndAttach(bus, dev, fun))
               {
                  printf("Unkown device: %x:%x:%x %s: 0x%0hx:0x%0hx\n", bus, dev, fun, getClassName(deviceClass), vid, did);
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
   
   driver->next = driversM;
   driversM = driver;
}

bool
Pci::probeAndAttach(uint8_t bus, uint8_t device, uint8_t function)
{
   PciDriver* driver = driversM;
   while (driver != 0)
   {
      // TODO attach 'best'
      if (driver->probe(bus, device, function) > 0)
      {
         printf("Initalizing driver %s\n", driver->name());
         driver->init(bus, device, function);

         return true;
      }

      driver = driver->next;
   }

   return false;
}
