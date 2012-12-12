/* 
 * File:   VgaConsole.h
 * Author: edmmhka
 *
 * Created on August 17, 2012, 1:36 PM
 */

#ifndef VGACONSOLE_H
#define	VGACONSOLE_H

#include <Console/Console.h>

typedef unsigned short VgaCharacter;

class VgaConsole : public Console {
public:
	VgaConsole();
	
	virtual ~VgaConsole();
	
	int getChar();
	int getColumns();
	int getRows();
	void putChar(int ch, int row, int column);
	using Console::putChar;
private:
	VgaConsole(const VgaConsole& orig);

	static constexpr VgaCharacter* vram = (VgaCharacter* )0xb8000;
};

#endif	/* VGACONSOLE_H */

