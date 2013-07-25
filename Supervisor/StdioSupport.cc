#include <Console.hh>

// XXX
#include <X86/SerialConsole.hh>
#include <StdioSupport.hh>

int
system_putchar(int ch)
{
   return SerialConsole::putChar(ch);
}

int
system_puts(const char* string)
{
   int count = 0;

   while (*string)
   {
      SerialConsole::putChar(*string++);
      count++;
   }

   return 0;
}
