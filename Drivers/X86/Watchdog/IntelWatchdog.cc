#include <Pci.hh>
#include <Driver/PciDriver.hh>
#include <Debug.hh>

#include <cstdint>

class WatchdogPciDriver : public PciDriver
{
public:
   WatchdogPciDriver();
   ~WatchdogPciDriver();

   int probe(uint8_t bus, uint8_t device, uint8_t function);
   bool init(uint8_t bus, uint8_t device, uint8_t function);
   bool finalize();
   const char* name() const;
};

// register
static WatchdogPciDriver driver;

WatchdogPciDriver::WatchdogPciDriver()
{
}

WatchdogPciDriver::~WatchdogPciDriver()
{
}

int
WatchdogPciDriver::probe(uint8_t bus, uint8_t device, uint8_t function)
{
   uint16_t vendorId = Pci::getVendorId(bus, device, function);
   uint16_t deviceId = Pci::getDeviceId(bus, device, function);

   if (vendorId == 0x8086 && deviceId == 0x25ab)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

bool
WatchdogPciDriver::init(uint8_t bus, uint8_t device, uint8_t function)
{
   uint32_t bar0 = Pci::readConfigurationRegister32(bus, device, function, Pci::Config::Bar0);
   Pci::writeConfigurationRegister32(bus, device, function, Pci::Config::Bar0, 0xffffffff);
   uint32_t barSize = Pci::readConfigurationRegister32(bus, device, function, Pci::Config::Bar0);
   if (barSize & 0x1)
   {
      // io
      barSize &= ~0x1;
      barSize = ~barSize + 1;
   }
   else
   {
      // mmio
      barSize &= ~0x7;
      barSize = ~barSize + 1;

   }
   Pci::writeConfigurationRegister32(bus, device, function, Pci::Config::Bar0, bar0);

   KASSERT(bar0 == Pci::readConfigurationRegister32(bus, device, function, Pci::Config::Bar0));

   driverInfo("bar0: 0x%x, %lu\n", bar0, barSize);

   return true;
}

bool
WatchdogPciDriver::finalize()
{
   return true;
}

const char*
WatchdogPciDriver::name() const
{
   return "i6300esb";
}
