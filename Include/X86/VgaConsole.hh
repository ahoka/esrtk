#ifndef VGACONSOLE_H
#define	VGACONSOLE_H

#include <Mutex.hh>
#include <Console.hh>
#include <stdint.h>

typedef uint16_t VgaCharacter;

class VgaConsole : public Console
{
public:
   VgaConsole();
   virtual ~VgaConsole();

   int init();

   int getColumns();
   int getRows();
   int putChar(int ch, int row, int column);
   void setCursor(int row, int column);
   void scrollScreen();
   void clearScreen();
   using Console::putChar;

private:
   VgaConsole(const VgaConsole& orig) = delete;
   VgaConsole& operator=(const VgaConsole& orig) = delete;

   int putCharUnlocked(int ch, int row, int column);
   VgaCharacter asciiToVga(uint8_t c);

   VgaCharacter*
   vram()
   {
      return (VgaCharacter* )vram_;
   }

   uint32_t vram_;
   Mutex lock;

//   backgroundColor(0x1f);
//   foregroundColor(0xff);

   enum VgaColors
   {
      Black = 0,
      Blue = 1,
      Green = 2,
      Cyan = 3,
      Red = 4,
      Magenta = 5,
      Brown = 6,
      LightGray = 7,
      DarkGray = 8,
      BrightBlue = 9,
      BrightGreen = 10,
      BrightCyan = 11,
      Pink = 12,
      BrightMagenta = 13,
      Yellow = 14,
      White = 15
   };

//   enum
//   {
//      backgroundColor = BrightBlue,
//      foregroundColor = White
//   };
   uint8_t backgroundColor;
   uint8_t foregroundColor;

   enum
   {
      VgaIndexPort = 0x3d4,
      VgaDataPort = 0x3d5,
   };

   enum
   {
      VgaCursorHigh = 14,
      VgaCursorLow = 15
   };
};

#endif	/* VGACONSOLE_H */
