#include "VgaConsole.hh"
#include "IoPort.hh"

VgaConsole::VgaConsole()
   : backgroundColor(0x1f),
     foregroundColor(0xff)
{
   clearScreen();
}

VgaConsole::VgaConsole(const VgaConsole& /*orig*/)
{
}

VgaConsole::~VgaConsole()
{
}

int
VgaConsole::getColumns()
{
   return 80;
}

int
VgaConsole::getRows()
{
   return 25;
}

int
VgaConsole::getChar()
{
   return 'a';
}

int
VgaConsole::putChar(int ch, int row, int column)
{
   if (row > getRows() + 1 || column > getColumns() + 1)
   {
      // error!
      return 0;
   }

   short repr = (ch & foregroundColor) | (backgroundColor << 8);

   vram()[row * getColumns() + column] = repr;

   return 1;
}

void
VgaConsole::clearScreen()
{
   for (int column = 0; column < getColumns(); column++)
   {
      for (int row = 0; row < getRows(); row++)
      {
	 putChar(' ', row, column);
      }
   }
}

void
VgaConsole::scrollScreen()
{
   for (int i = 0; i < getRows() * (getColumns() - 1); i++)
   {
      vram()[i] = vram()[i + getColumns()];
   }
   
   for (int i = (getRows() - 1) * getColumns();
	i < getRows() * getColumns();
	i++)
   {
      vram()[i] = (' ' & foregroundColor) | (backgroundColor << 8);
   }
}

void
VgaConsole::setCursor(int row, int column)
{
   uint16_t cursorIndex = row * getColumns() + column;

   outb(VgaIndexPort, VgaCursorLow);
   outb(VgaDataPort, cursorIndex & 0xff);

   outb(VgaIndexPort, VgaCursorHigh);
   outb(VgaDataPort, (cursorIndex >> 8) & 0xff);
}
