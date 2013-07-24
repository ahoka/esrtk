#include <X86/SerialConsole.hh>
#include <X86/IoPort.hh>

int
SerialConsole::getChar()
{
   char ch;

   ch = inb(0x3f8);

   return ch;
}

int
SerialConsole::putChar(int ch)
{
   outb(0x3f8, (uint8_t)ch);
   
   if (ch == '\n')
   {
      outb(0x3f8, '\r');
   }

   return 1;
}
