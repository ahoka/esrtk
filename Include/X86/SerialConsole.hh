#ifndef SERIALCONSOLE_H
#define	SERIALCONSOLE_H

class SerialConsole {
public:
   static int getChar();
   static int putChar(int ch);

private:
   SerialConsole();
   virtual ~SerialConsole();
   SerialConsole(const SerialConsole& orig);
   SerialConsole& operator=(const SerialConsole& orig);

   enum
   {
      SerialDataPort = 0x3f8
   };
};

#endif	/* SERIALCONSOLE_H */
