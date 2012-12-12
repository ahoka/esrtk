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
Console::write(const char* /*string*/)
{
	//
}

void
Console::putChar(char ch)
{
   putChar(ch, currentColumn, currentRow);
   
   currentColumn++;
   if (currentColumn > getColumns() - 1) {
      currentColumn = 0;
      currentRow++;
      
      if (currentRow > getRows() - 1) {
	 // generic scroll or smart scroll by driver?
	 //scrollScreen();
	 currentRow = 0; // just flow over to the top now
      }
   }
}
