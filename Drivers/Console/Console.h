#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdarg.h>

#ifdef HAS_LONGLONG
typedef unsigned long long  printf_uint_t;
typedef long long printf_int_t;
#else
typedef unsigned long printf_uint_t;
typedef long printf_int_t;
#endif

class Console {
public:
   Console();
   virtual ~Console();

   void putChar(char ch);
   int putString(const char* str);
   virtual void putChar(int ch, int row, int column) = 0;
   virtual void clearScreen() = 0;
   virtual void setCursor(int row, int column) = 0;
   virtual void scrollScreen() = 0;

   // doesnt belong here
   int printf(const char* format, ...);

protected:
   virtual int getRows() = 0;
   virtual int getColumns() = 0;
   virtual int getChar() = 0;

private:
   static const int printfBufferSize = 128;
   int doVaPrint(va_list* ap, int type, int modifiers, int flags);
   void stepRow();
   void stepColumn();
   int currentRow;
   int currentColumn;
};

#endif
