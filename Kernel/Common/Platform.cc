#include <SystemTimer.hh>
#include <Driver/DriverManager.hh>
#include <Kernel/Pci.hh>
#include <Clock.hh>
#include <Modules.hh>

#include <Platform.hh>

#include <AcpiCa.hh>

#include <cstdio>

void
Platform::init()
{
#if 0
   char id[13];
   cpuid0(id);

   printf("CPU Vendor ID: %s\n", id);
#endif
//   Acpi::printAllDescriptors();
   ACPI_STATUS status;
   status = AcpiInitializeSubsystem();
   if (ACPI_FAILURE(status))
   {
//      Debug::error("Error: AcpiInitializeSubsystem\n");
   }
   
   AcpiInitializeTables(NULL, 0, true);
   // AcpiLoadTables();
   //AcpiEnableSubsystem(ACPI_FULL_INITIALIZATION);
   //AcpiInitializeObjects(ACPI_FULL_INITIALIZATION);

   SystemTimer::probeAndInit();
   Clock::probeAndInit();
   
   Pci::init();
   DriverManager::probeAndInit();
   Pci::enumerate();
   Modules::handleModules();

   printf("Platform init completed\n");
}
