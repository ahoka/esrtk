#include "SerialConsole.hh"
#include "IoPort.hh"

SerialConsole::SerialConsole()
{
}

SerialConsole::SerialConsole(const SerialConsole& /*orig*/)
{
}

SerialConsole::~SerialConsole()
{
}

int
SerialConsole::getColumns()
{
   return 80;
}

int
SerialConsole::getRows()
{
   return 25;
}

int
SerialConsole::getChar()
{
   char ch;

   ch = inb(0x3f8);

   return ch;
}

int
SerialConsole::putChar(int ch, int row, int column)
{
   outb(0x3f8, (uint8_t)ch);
   
   if (ch == '\n')
   {
      outb(0x3f8, '\r');
   }

   return 1;
}

void
SerialConsole::clearScreen()
{
}

void
SerialConsole::scrollScreen()
{
   outb(0x3f8, '\n');
   outb(0x3f8, '\r');
}

void
SerialConsole::setCursor(int row, int column)
{
}
