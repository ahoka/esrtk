#ifndef VGACONSOLE_HH
#define	VGACONSOLE_HH

#include <Mutex.hh>
#include <Driver/FrameBuffer.hh>

#include <cstddef>
#include <cstdint>

typedef uint16_t VgaCharacter;

class VgaConsole final : public FrameBuffer
{
public:
   VgaConsole();
   virtual ~VgaConsole();

   void init();

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
   VgaCharacter asciiToVga(uint8_t c) const;

   size_t vramSizeM;
   volatile VgaCharacter* vramM;

   mutable Mutex lock;
   bool isInitialized;

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

   enum
   {
      VgaColumns = 80,
      VgaRows = 25
   };
};

#endif
