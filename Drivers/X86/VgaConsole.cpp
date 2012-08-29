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