#include <X86/VgaConsole.hh>
#include <X86/IoPort.hh>
#include <Memory.hh>
#include <Debug.hh>

//extern VgaConsole vgaConsole;
static VgaConsole vgaConsole;

VgaConsole::VgaConsole()
   : vramSizeM(VgaColumns * VgaRows * sizeof(VgaCharacter)),
     vramM(0),
     lock(),
     backgroundColor(Blue),
     foregroundColor(White)
{
   // TODO: MAP AS WC!
   vramM = reinterpret_cast<VgaCharacter*>(Memory::mapRegion(0xb8000u, vramSizeM, Memory::MapUncacheable));
   KASSERT(vramM != 0);

   clearScreen();
   Console::enable();
}

VgaConsole::~VgaConsole()
{
   Memory::unmapRegion(reinterpret_cast<uintptr_t>(vramM), vramSizeM);
}

int
VgaConsole::getColumns()
{
   return VgaColumns;
}

int
VgaConsole::getRows()
{
   return VgaRows;
}

VgaCharacter
VgaConsole::asciiToVga(uint8_t c) const
{
   return (uint16_t)(c | (backgroundColor << 4 | foregroundColor) << 8);
}

int
VgaConsole::putCharUnlocked(int ch, int row, int column)
{
   if (row > getRows() + 1 || column > getColumns() + 1)
   {
      // error!
      return 0;
   }

   vramM[row * getColumns() + column] = asciiToVga(ch);

   return 1;
}

int
VgaConsole::putChar(int ch, int row, int column)
{
   // bool l = lock.tryEnter();
   // if (!l)
   // {
   //    return 0;
   // }

   int result = putCharUnlocked(ch, row, column);

   return result;
}

void
VgaConsole::clearScreen()
{
   for (int column = 0; column < getColumns(); column++)
   {
      for (int row = 0; row < getRows(); row++)
      {
	 putCharUnlocked(' ', row, column);
      }
   }

   setCursor(0, 0);
}

void
VgaConsole::scrollScreen()
{
   lock.enter();

   for (int i = 0; i < getRows() * (getColumns() - 1); i++)
   {
      vramM[i] = vramM[i + getColumns()];
   }
   
   for (int i = (getRows() - 1) * getColumns();
	i < getRows() * getColumns();
	i++)
   {
      vramM[i] = asciiToVga(' ');
   }

   lock.exit();
}

void
VgaConsole::setCursor(int row, int column)
{
   lock.enter();

   uint16_t cursorIndex = (uint16_t )(row * getColumns() + column);

   outb(VgaIndexPort, VgaCursorLow);
   outb(VgaDataPort, cursorIndex & 0xff);

   outb(VgaIndexPort, VgaCursorHigh);
   outb(VgaDataPort, (cursorIndex >> 8) & 0xff);

   lock.exit();
}
