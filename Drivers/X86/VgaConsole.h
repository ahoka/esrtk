#ifndef VGACONSOLE_H
#define	VGACONSOLE_H

#include <Console/Console.h>

const int VGA_VRAM_BASE = 0xb8000;

enum {
   VGA_INDEX_PORT = 0x3d4,
   VGA_DATA_PORT = 0x3d5,
};

enum {
   VGA_CURSOR_HIGH = 14,
   VGA_CURSOR_LOW = 15
};

typedef unsigned short VgaCharacter;

class VgaConsole : public Console {
public:
	VgaConsole();
	virtual ~VgaConsole();

	int getChar();
	int getColumns();
	int getRows();
	void putChar(int ch, int row, int column);
	void setCursor(int row, int column);
	void scrollScreen();
	void clearScreen();
	using Console::putChar;

private:
	VgaConsole(const VgaConsole& orig);

	static constexpr VgaCharacter* vram = (VgaCharacter* )VGA_VRAM_BASE;
};

#endif	/* VGACONSOLE_H */
