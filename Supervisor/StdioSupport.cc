#include <Console.hh>

// XXX
#include <X86/SerialConsole.hh>
#include <StdioSupport.hh>

int
system_putchar(int ch)
{
   Console* con = Console::consoleList;
   while (con != 0)
   {
      con->putChar((char)ch);
      con = con->next;
   }

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

   Console* con = Console::consoleList;
   while (con != 0)
   {
      con->putString(string);
      con = con->next;
   }

   return 0;
}
