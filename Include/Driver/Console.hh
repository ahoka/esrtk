#ifndef CONSOLE_HH
#define CONSOLE_HH

#include <stdarg.h>

class Console
{
public:
   Console();
   virtual ~Console();

   virtual void putChar(char ch) = 0;

   bool isEnabled() const;
   void enable();
   void disable();

private:
   Console(const Console&) = delete;
   Console& operator=(const Console&) = delete;

   bool isEnabledM;
};

#endif
