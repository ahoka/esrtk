#include <X86/VgaConsole.hh>
#include <X86/IoPort.hh>
#include <Memory.hh>
#include <Debug.hh>

extern VgaConsole vgaConsole;
VgaConsole vgaConsole;

VgaConsole::VgaConsole()
{
   vram_ = Memory::mapPage(0xb8000);
   KASSERT(vram_ != 0);

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

   uint16_t repr = (uint16_t )((ch & foregroundColor) | (backgroundColor << 8));

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
      vram()[i] = (VgaCharacter )((' ' & foregroundColor) | (backgroundColor << 8));
   }
}

void
VgaConsole::setCursor(int row, int column)
{
   uint16_t cursorIndex = (uint16_t )(row * getColumns() + column);

   outb(VgaIndexPort, VgaCursorLow);
   outb(VgaDataPort, cursorIndex & 0xff);

   outb(VgaIndexPort, VgaCursorHigh);
   outb(VgaDataPort, (cursorIndex >> 8) & 0xff);
}
