#include "Console.h"

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
   PRINTF_FLAG_UPPERHEX = 1 << 3
};

int
Console::printf(const char* format, ...)
{
   int retval = 0;

   while (*format != 0)
   {
      if (format != '%')
      {
	 putChar(*format++);
	 retval++;
      }
      else
      {
	 // let's see what's after %
	 format++; 

	 // "%d %llu %hd"

	 int flags = 0;
	 switch (*format)
	 {
	    case 'd':
	       flags |= PRINTF_FLAG_SIGNED;
	       format++;
	       break;
	    case 'u':
	       flags |= PRINTF_FLAG_UNSIGNED;
	       format++;
	       break;
	    case 'x':
	       flags |= PRINTF_FLAG_LOWERHEX;
	       format++;
	       break;
	    case 'X':
	       flags |= PRINTF_FLAG_UPPERHEX;
	       format++;
	       break;
	    default:
	       // just print whatever it was...	
	       putChar(*format++);
	       retval++;
	 }
	 
      }
   }
}
