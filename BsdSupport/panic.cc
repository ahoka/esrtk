#include <Debug.hh>
#include <cstdarg>
#include <cstdio>

extern "C" void
panic(const char *fmt, ...)
{
   va_list ap;
   
   va_start(ap, fmt);
   vprintf(fmt, ap);
   
   Debug::panic("BSD panic() called");
}
