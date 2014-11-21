#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdarg.h>

class Console {
public:
   Console();
   virtual ~Console();

   int putChar(char ch);
   int putString(const char* str);
   int write(const char* str, int size);

   bool isEnabled() const;
   void enable();
   void disable();

protected:
   virtual int putChar(int ch, int row, int column) = 0;
   virtual void clearScreen() = 0;
   virtual void setCursor(int row, int column) = 0;
   virtual void scrollScreen() = 0;

   virtual int getRows() = 0;
   virtual int getColumns() = 0;

private:
   Console(const Console&) = delete;
   Console& operator=(const Console&) = delete;

   void init();

   static const int printfBufferSize = 128;
   int doVaPrint(va_list* ap, int type, int modifiers, int flags);
   void stepRow();
   void stepColumn();
   int currentRow;
   int currentColumn;
   bool isEnabledM;
};

#endif
