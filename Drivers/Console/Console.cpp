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
