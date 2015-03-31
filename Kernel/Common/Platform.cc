#include <SystemTimer.hh>
#include <Driver/DriverManager.hh>
#include <Kernel/Pci.hh>
#include <Clock.hh>
#include <Modules.hh>

#include <Platform.hh>

#include <AcpiCa.hh>

void
Platform::init()
{
#if 0
   char id[13];
   cpuid0(id);

   printf("CPU Vendor ID: %s\n", id);
#endif
//   Acpi::printAllDescriptors();
   AcpiInitializeSubsystem();

   SystemTimer::probeAndInit();
   Clock::probeAndInit();
   
   Pci::init();
   DriverManager::probeAndInit();
   Pci::enumerate();
   Modules::handleModules();
}
