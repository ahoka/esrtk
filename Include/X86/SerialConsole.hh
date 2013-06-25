#ifndef SERIALCONSOLE_H
#define	SERIALCONSOLE_H

#include <Console.hh>
#include <stdint.h>

typedef uint16_t SerialCharacter;

class SerialConsole : public Console {
public:
   SerialConsole();
   virtual ~SerialConsole();

   int getChar();
   int getColumns();
   int getRows();
   int putChar(char ch);
   int putChar(int ch, int row, int column);
   void setCursor(int row, int column);
   void scrollScreen();
   void clearScreen();
   using Console::putChar;

private:
   SerialConsole(const SerialConsole& orig);
   SerialConsole& operator=(const SerialConsole& orig);

   enum {
      SerialDataPort = 0x3f8
   };
};

#endif	/* SERIALCONSOLE_H */
