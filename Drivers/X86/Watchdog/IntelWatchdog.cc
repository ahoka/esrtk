#include <Pci.hh>
#include <Driver/PciDriver.hh>

#include <Interrupt.hh>
#include <Debug.hh>
#include <Memory.hh>

#include <WatchdogDevice.hh>

#include <cstdint>

class WatchdogPciDriver : public PciDriver, private WatchdogDevice
{
public:
   WatchdogPciDriver();
   ~WatchdogPciDriver();

   int probe(uint8_t bus, uint8_t device, uint8_t function);
   bool init(uint8_t bus, uint8_t device, uint8_t function);
   bool finalize();
   const char* name() const;

   void kick();

private:
   enum ConfigurationSpace
   {
      WdtConfiguration = 0x60,
      WdtLockRegister = 0x68
   };

   enum BarRegisters
   {
      PreloadValue1 = 0x00,
      PreloadValue2 = 0x04,
      GeneralInterruptStatus = 0x08,
      ReloadRegister = 0x0c
   };

   uintptr_t mmioM;
   size_t mmioSizeM;

   uint32_t read32(unsigned int offset)
   {
      return *(volatile uint32_t*)(mmioM + offset);
   }

   void write32(unsigned int offset, uint32_t value)
   {
      *(volatile uint32_t*)(mmioM + offset) = value;
      __sync_synchronize();
   }

   uint16_t read16(unsigned int offset)
   {
      return *(volatile uint16_t*)(mmioM + offset);
   }

   void write16(unsigned int offset, uint16_t value)
   {
      *(volatile uint16_t*)(mmioM + offset) = value;
      __sync_synchronize();
   }

   uint8_t read8(unsigned int offset)
   {
      return *(volatile uint8_t*)(mmioM + offset);
   }

   void write8(unsigned int offset, uint8_t value)
   {
      *(volatile uint8_t*)(mmioM + offset) = value;
      __sync_synchronize();
   }

   void unlock()
   {
      write8(ReloadRegister, 0x80);
      write8(ReloadRegister, 0x86);
   }

   class WatchdogInterruptHandler : public InterruptHandler
   {
   private:
      WatchdogPciDriver* driverM;

   public:
      WatchdogInterruptHandler(WatchdogPciDriver* driver)
         : driverM(driver)
      {
      }

      void handleInterrupt()
      {
         if (driverM->read8(GeneralInterruptStatus) & 0x01)
         {
            driverM->driverInfo("Watchdog timer expired!\n");
         }
      }
   };
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

   uint8_t interruptPin = Pci::readConfigurationRegister8(bus, device, function, Pci::Config::InterruptPin);
   uint8_t interruptLine = Pci::readConfigurationRegister8(bus, device, function, Pci::Config::InterruptLine);

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
      driverInfo("bar0: %s 0x%x-0x%x (%u)\n", type, bar0, bar0 + barSize, barSize);
   }
   else
   {
      driverInfo("bar0: %s 0x%x-0x%x (%u), IRQ%u (%s)\n", type, (bar0 & ~0x7), (bar0 & ~0x7) + barSize, barSize, interruptLine, pins[interruptPin]);
   }

   Pci::writeConfigurationRegister32(bus, device, function, Pci::Config::Bar0, bar0);

   KASSERT(bar0 == Pci::readConfigurationRegister32(bus, device, function, Pci::Config::Bar0));

   mmioM = Memory::mapRegion(bar0 & ~0x7, barSize, Memory::MapUncacheable);

   driverInfo("Registers mapped to %p\n", (void*)mmioM);

   bool success = Interrupt::registerHandler(10, new WatchdogInterruptHandler(this));
   if (!success)
   {
      driverInfo("Registering interrupt handler failed!\n");
   }

   driverInfo("Default values: PRE1: %u, PRE2: %u\n", read32(PreloadValue1), read32(PreloadValue2));

   uint8_t lockRegister = Pci::readConfigurationRegister8(bus, device, function, WdtLockRegister);
   driverInfo("Lock register: 0x%x\n", lockRegister);

   uint16_t configRegister = Pci::readConfigurationRegister16(bus, device, function, WdtConfiguration);
   driverInfo("Configuration register: 0x%x\n", configRegister);

   driverInfo("Interrupt status: 0x%x\n", read8(GeneralInterruptStatus));
   driverInfo("Reload register: 0x%x\n", read16(ReloadRegister));

   unlock();
   write32(PreloadValue1, 5000);

   unlock();
   write32(PreloadValue2, 5000);

   unlock();
   write16(ReloadRegister, 1 << 8);

   driverInfo("New values: PRE1: %u, PRE2: %u\n", read32(PreloadValue1), read32(PreloadValue2));

   Pci::writeConfigurationRegister8(bus, device, function, WdtLockRegister, 0x3);

   registerWatchdog();
   driverInfo("Watchdog armed!\n");

   return true;
}

void
WatchdogPciDriver::kick()
{
   unlock();
   write16(ReloadRegister, 1 << 8);
}

bool
WatchdogPciDriver::finalize()
{
   unregisterWatchdog();
   Memory::unmapRegion(mmioM, 16);
   mmioM = 0;

   return true;
}

const char*
WatchdogPciDriver::name() const
{
   return "i6300esb";
}
