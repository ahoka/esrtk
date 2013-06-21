#include "Console.hh"

#include <stdarg.h>

Console::Console()
{
   init();
}

Console::~Console()
{
}

int
Console::init()
{
   currentRow = 0;
   currentColumn = 0;
}

int
Console::putString(const char* str)
{
   int retval = 0;

   while (*str)
   {
      putChar(*str++);
      retval++;
   }

   setCursor(currentRow, currentColumn);

   return retval;
}

int
Console::write(const char* str, int size)
{
   for (int i = 0; i < size; i++)
   {
      putChar(*str++);
   }

   setCursor(currentRow, currentColumn);

   return size;
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

#define TABSIZE 8

int
Console::putChar(char ch)
{
   int retval = 0;

   switch (ch)
   {
      case '\n':
         retval++;
	 stepRow();
	 currentColumn = 0;
	 break;
      case '\r':
         retval++;
	 break;
      case '\t':
      {
	 // TODO Refactor!
	 int newcol = ((currentColumn + TABSIZE) / TABSIZE) * TABSIZE;
	 int steps = newcol - currentColumn;
	 for (int i = 0; i < steps; i++)
	 {
            retval++;
	    stepColumn();
	 }
	 break;
      }
      default:
	 putChar(ch, currentRow, currentColumn);
         retval++;
	 stepColumn();
	 break;
   }

   /// XXX make this smarter
   setCursor(currentRow, currentColumn);

   return  retval;
}
