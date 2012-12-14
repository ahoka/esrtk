#include "Console.h"

#include <stdarg.h>

Console::Console()
  : currentRow(0),
    currentColumn(0)
{
}

Console::~Console()
{
}

void
Console::write(const char* string)
{
   while (*string)
   {
      putChar(*string++);
   }
   
   setCursor(currentRow, currentColumn);
}

void
Console::stepRow()
{
   currentRow++;
  
   if (currentRow > getRows() - 1)
   {
      // generic scroll or smart scroll by driver?
      scrollScreen();
      currentColumn = 0;
      currentRow = getRows() - 1; // just flow over to the top now
      setCursor(getRows() - 1, 0);
   }
}

void
Console::stepColumn()
{
      currentColumn++;
      if (currentColumn > getColumns() - 1) {
	 currentColumn = 0;
	 stepRow();
      }
}

void
Console::putChar(char ch)
{
   switch (ch)
   {
      case '\n':
	 stepRow();
	 currentColumn = 0;
	 break;
      case '\r':
	 break;
      default:
	 putChar(ch, currentRow, currentColumn);
	 stepColumn();
	 break;
   }
}

enum {
   PRINTF_FLAG_SIGNED = 1,
   PRINTF_FLAG_UNSIGNED = 1 << 1,
   PRINTF_FLAG_LOWERHEX = 1 << 2,
   PRINTF_FLAG_UPPERHEX = 1 << 3,
   PRINTF_FLAG_LONG = 1 << 4,
   PRINTF_FLAG_LONGLONG = 1 << 5,
   PRINTF_FLAG_SHORT = 1 << 6,
   PRINTF_FLAG_SHORTSHORT = 1 << 7
};

#define PRINTF_PUTCHAR(x) (putChar(x), retval++)

int
Console::printf(const char* format, ...)
{
   va_list ap;
   int retval = 0;

   va_start(ap, format);

   while (*format != 0)
   {
      if (*format != '%')
      {
	 putChar(*format++);
	 retval++;
      }
      else
      {
	 int flags = 0;
	 // let's see what's after %
	 // "%d %llu %hd"
	 bool finished = false;
	 while (!finished && *++format != 0)
	 {
	    switch (*format)
	    {
	       case 'l':
		  if (flags & PRINTF_FLAG_LONGLONG ||
		      flags & PRINTF_FLAG_SHORT)
		  {
		     finished = true;
		     // TODO print all stuff interpreted so far
		     PRINTF_PUTCHAR(*format++);
		     // putChar(*format++);
		     // retval++;
		  }
		  else if (flags & PRINTF_FLAG_LONG)
		  {
		     flags |= PRINTF_FLAG_LONGLONG;
		  }
		  else
		  {
		     flags |= PRINTF_FLAG_LONG;
		  }
		  break;
	       case 'h':
		  if (flags & PRINTF_FLAG_LONG ||
		      flags & PRINTF_FLAG_SHORTSHORT)
		  {
		     finished = true;
		     // TODO print all stuff interpreted so far
		     PRINTF_PUTCHAR(*format++);
		     // putChar(*format++);
		     // retval++;
		  }
		  else if (flags & PRINTF_FLAG_SHORT)
		  {
		     flags |= PRINTF_FLAG_SHORTSHORT;
		  }
		  else
		  {
		     flags |= PRINTF_FLAG_SHORT;
		  }
		  break;
	       case 'd':
		  flags |= PRINTF_FLAG_SIGNED;
		  finished = true;
		  break;
	       case 'u':
		  flags |= PRINTF_FLAG_UNSIGNED;
		  finished = true;
		  break;
	       case 'x':
		  flags |= PRINTF_FLAG_LOWERHEX;
		  finished = true;
		  break;
	       case 'X':
		  flags |= PRINTF_FLAG_UPPERHEX;
		  finished = true;
		  break;
	       default:
		  // something unexpected
		  // just print whatever it was...
		  finished = true;
		  PRINTF_PUTCHAR(*format++);
		  // putChar(*format++);
		  // retval++;
	    }
	 }
      }
   }

   va_end(ap);

   return retval;
}
