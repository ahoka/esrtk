#include <Driver/FrameBuffer.hh>
#include <Driver/Console.hh>

#include <stdarg.h>
#include <cstdio>

FrameBuffer::FrameBuffer()
   : currentRow(0),
     currentColumn(0)
{
}

FrameBuffer::~FrameBuffer()
{
}

void
FrameBuffer::stepRow()
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
FrameBuffer::stepColumn()
{
      currentColumn++;
      if (currentColumn > getColumns() - 1) {
	 currentColumn = 0;
	 stepRow();
      }
}

#define TABSIZE 8

void
FrameBuffer::putChar(char ch)
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
}
