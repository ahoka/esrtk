/* 
 * File:   VgaConsole.h
 * Author: edmmhka
 *
 * Created on August 17, 2012, 1:36 PM
 */

#ifndef VGACONSOLE_H
#define	VGACONSOLE_H

#include "../Console/ConsoleInterface.hh"

class VgaConsole : ConsoleInterface {
public:
	VgaConsole();
	
	virtual ~VgaConsole();
	
	int getChar(int x, int y);
	int getColumns();
private:
	VgaConsole(const VgaConsole& orig);

	static const unsigned long vram = 0xb8000;
};

#endif	/* VGACONSOLE_H */

