#include <Pci.hh>
#include <Driver/PciDriver.hh>
#include <Debug.hh>
#include <Memory.hh>

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

private:
   enum ConfigurationSpace
   {
      WdtConfiguration = 0x60,
      WdtLockRegister = 0x68
   };

   enum BarRegisters
   {
      PreloadValue1 = 0,
      PreloadValue2 = 1,
      GeneralInterruptStatus = 2,
      ReloadRegister = 3
   };

   uint32_t* registersM;
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

   uint8_t interruptPin = Pci::readConfigurationRegister32(bus, device, function, Pci::Config::InterruptPin);
   uint8_t interruptLine = Pci::readConfigurationRegister32(bus, device, function, Pci::Config::InterruptLine);

   const char* pins[] = {
      "None",
      "INTA#",
      "INTB#",
      "INTC#",
      "INTD#",
   };

   Pci::writeConfigurationRegister32(bus, device, function, Pci::Config::Bar0, 0xffffffff);
   uint32_t barSize = Pci::readConfigurationRegister32(bus, device, function, Pci::Config::Bar0);

   const char* type;
   if (barSize & 0x1)
   {
      // io
      barSize &= ~0x1;
      barSize = ~barSize + 1;
      type = "IO";
   }
   else
   {
      // mmio
      barSize &= ~0x7;
      barSize = ~barSize + 1;
      type = "MMIO";
   }

   if (interruptPin == 0)
   {
      driverInfo("bar0: %s 0x%x-0x%x (%lu)\n", type, bar0, bar0 + barSize, barSize);
   }
   else
   {
      driverInfo("bar0: %s 0x%x-0x%x (%lu), IRQ%lu (%s)\n", type, bar0, bar0 + barSize, barSize, interruptLine, pins[interruptPin]);
   }

   Pci::writeConfigurationRegister32(bus, device, function, Pci::Config::Bar0, bar0);

   KASSERT(bar0 == Pci::readConfigurationRegister32(bus, device, function, Pci::Config::Bar0));

   registersM = (uint32_t*)Memory::mapAnonymousPage(bar0 & ~0x7, Memory::MapUncacheable);

   driverInfo("Registers mapped to %p\n", registersM);

   registersM[ReloadRegister] = 0x80;
   registersM[ReloadRegister] = 0x86;

   registersM[PreloadValue1] = 0xffffffff;

   registersM[ReloadRegister] = 0x80;
   registersM[ReloadRegister] = 0x86;

   registersM[PreloadValue2] = 0xffffffff;

   registersM[ReloadRegister] = 0x80;
   registersM[ReloadRegister] = 0x86;

   registersM[WdtLockRegister] = 1 << 1;

   driverInfo("Watchdog armed!\n");

   return true;
}

bool
WatchdogPciDriver::finalize()
{
   Memory::unmapPage((uintptr_t)registersM);
   registersM = 0;

   return true;
}

const char*
WatchdogPciDriver::name() const
{
   return "i6300esb";
}
