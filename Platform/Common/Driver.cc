#include <Driver/Driver.hh>
#include <Driver/DriverManager.hh>
#include <cstdio>

Driver::Driver() : 
   state(State::Uninitalized),
   next(0)
{
   DriverManager::registerDriver(this);
}

Driver::~Driver()
{
   // dummy
   state = State::Uninitalized;
}

int
Driver::driverInfo(const char* format, ...)
{
   va_list va;
   va_start(va, format);

   // XXX todo sprintf
   int ret = printf("%s: ", name());
   ret += vprintf(format, va);

   va_end(va);

   return ret;
}
