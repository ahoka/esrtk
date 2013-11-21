/* 
 * File:   Debug.cpp
 * Author: edmmhka
 * 
 * Created on August 17, 2012, 1:39 PM
 */

#include <Debug.hh>
#include <StackTrace.hh>
#include <Power.hh>
#include <cstdio>

Debug::DebugLevel Debug::debugLevel = Debug::DebugLevel::Info;

void
Debug::panic(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   printf("Supervisor Error: ");
   vprintf(message, va);
   printf("\n");

   va_end(va);

   StackTrace::printStackTrace();

   printf("\nYour adventure ends here.\n");

   Power::halt();
}

void
Debug::info(const char* message, ...)
{
   if (debugLevel < DebugLevel::Info)
   {
      return;
   }

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

   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}
