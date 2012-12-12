/* 
 * File:   VgaConsole.cpp
 * Author: edmmhka
 * 
 * Created on August 17, 2012, 1:36 PM
 */

#include "VgaConsole.h"

VgaConsole::VgaConsole()
{
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
