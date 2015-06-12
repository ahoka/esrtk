#include <Driver/PciDriver.hh>
#include <Kernel/Pci.hh>
#include <cstdio>

PciDriver::PciDriver() : 
   next(0)
{
   Pci::registerDriver(this);
}

PciDriver::~PciDriver()
{
}

int
PciDriver::driverInfo(const char* format, ...)
{
   va_list va;
   va_start(va, format);

   // XXX todo sprintf
   int ret = printf("%s: ", name());
   ret += vprintf(format, va);

   va_end(va);

   return ret;
}
