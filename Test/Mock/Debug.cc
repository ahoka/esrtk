#include <Debug.hh>

#include <cstdlib>
#include <cstdarg>
#include <cstdio>

void Debug::panic(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);

   abort();
}

void Debug::verbose(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}

void Debug::info(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}

void Debug::warning(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}

void Debug::error(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   vprintf(message, va);

   va_end(va);
}
