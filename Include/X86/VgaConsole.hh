#ifndef VGACONSOLE_H
#define	VGACONSOLE_H

#include <Console.hh>
#include <stdint.h>

typedef uint16_t VgaCharacter;

class VgaConsole : public Console {
public:
   VgaConsole();
   virtual ~VgaConsole();

   int getChar();
   int getColumns();
   int getRows();
   int putChar(int ch, int row, int column);
   void setCursor(int row, int column);
   void scrollScreen();
   void clearScreen();
   using Console::putChar;

private:
   VgaConsole(const VgaConsole& orig);
   VgaConsole& operator=(const VgaConsole& orig);

   VgaCharacter* const vram = reinterpret_cast<VgaCharacter*>(0xb8000);

   uint8_t backgroundColor;
   uint8_t foregroundColor;

   enum {
      VgaIndexPort = 0x3d4,
      VgaDataPort = 0x3d5,
   };

   enum {
      VgaCursorHigh = 14,
      VgaCursorLow = 15
   };
};

#endif	/* VGACONSOLE_H */
