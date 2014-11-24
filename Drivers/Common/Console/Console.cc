#include <Driver/Console.hh>
#include <StdioSupport.hh>

#include <stdarg.h>
#include <cstdio>

Console::Console()
   : isEnabledM(false)
{
   KernelStdio::registerConsole(this);
}

Console::~Console()
{
   KernelStdio::unregisterConsole(this);
}

void
Console::enable()
{
   isEnabledM = true;
}

void
Console::disable()
{
   isEnabledM = false;
}

bool
Console::isEnabled() const
{
   return isEnabledM;
}
