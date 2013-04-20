/* 
 * File:   VgaConsole.cpp
 * Author: edmmhka
 * 
 * Created on August 17, 2012, 1:36 PM
 */

#include "VgaConsole.h"
#include "IoPort.h"

VgaConsole::VgaConsole()
  :  vram((VgaCharacter* )VGA_VRAM_BASE)
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

void
VgaConsole::putChar(int ch, int row, int column)
{
   if (row > getRows() + 1 || column > getColumns() + 1)
   {
      // error!
      return;
   }

   short repr = (ch & 0xff) | (0x07 << 8);

   vram[row * getColumns() + column] = repr;
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
      vram[i] = vram[i + getColumns()];
   }
   
   for (int i = getRows() * (getColumns() - 1); i < getRows() * getColumns(); i++)
   {
      vram[i] = 0x0720;
   }
}

void
VgaConsole::setCursor(int row, int column)
{
   uint16_t cursorIndex = row * getColumns() + column;

   outb(VGA_INDEX_PORT, VGA_CURSOR_LOW);
   outb(VGA_DATA_PORT, cursorIndex & 0xff);

   outb(VGA_INDEX_PORT, VGA_CURSOR_HIGH);
   outb(VGA_DATA_PORT, (cursorIndex >> 8) & 0xff);
}
