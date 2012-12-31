#include <System.h>

#include <stdarg.h>
#include <stdio.h>

int printf(const char* format, ...)
{
   va_list ap;
   int retval;
   
   va_start(ap, format);
   
   retval = System::console.vprintf(format, ap);
   
   va_end(ap);
   
   return retval;
}

int vprintf(const char* format, va_list ap)
{
   int retval;

   retval = System::console.vprintf(format, ap);

   return retval;
}


