#include <SystemTimer.hh>
#include <DriverManager.hh>
#include <Pci.hh>
#include <Clock.hh>

#include <Platform.hh>

void
Platform::init()
{
#if 0
   char id[13];
   cpuid0(id);

   printf("CPU Vendor ID: %s\n", id);
#endif
//   Acpi::printAllDescriptors();

   SystemTimer::probeAndInit();
   Clock::probeAndInit();
   Pci::init();
   DriverManager::probeAndInit();
}
