#include <Debug.hh>
#include <Kernel/StackTrace.hh>
#include <Kernel/Power.hh>
#include <Kernel/Clock.hh>
#include <Kernel/Cpu.hh>

#include <cstdio>

Debug::DebugLevel Debug::debugLevel = Debug::DebugLevel::Verbose;

void
Debug::panic(const char* message, ...)
{
   Cpu::disableLocalInterrupts();

   va_list va;
   va_start(va, message);

   printf("Supervisor Error: ");
   vprintf(message, va);
   printf("\n");

   va_end(va);

   StackTrace::printStackTrace();

   printf("\nYour adventure ends here.\n");

   Power::shutdown();
}

void
Debug::printTimeStamp()
{
//   printf("[%llu] ", Clock::getTimeInMicroSeconds());
}

void
Debug::verbose(const char* message, ...)
{
   if (debugLevel < DebugLevel::Verbose)
   {
      return;
   }

   Debug::printTimeStamp();

   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}

void
Debug::info(const char* message, ...)
{
   if (debugLevel < DebugLevel::Info)
   {
      return;
   }

   Debug::printTimeStamp();

   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}

void
Debug::warning(const char* message, ...)
{
   if (debugLevel < DebugLevel::Warning)
   {
      return;
   }

   Debug::printTimeStamp();

   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}

void
Debug::error(const char* message, ...)
{
   if (debugLevel < DebugLevel::Error)
   {
      return;
   }

   Debug::printTimeStamp();

   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}
