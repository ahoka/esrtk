#include <Console.hh>

// XXX
#include <X86/SerialConsole.hh>
#include <StdioSupport.hh>

int
system_putchar(int ch)
{
   static bool nested;

   int rv = SerialConsole::putChar(ch);  

   // disallow nested calls
   //
   if (!nested)
   {
      nested = true;
      Console* con = Console::consoleList;
      while (con != 0)
      {
	 con->putChar((char)ch);
	 con = con->next;
      }
      nested = false;
   }

   return rv;
}

int
system_puts(const char* string)
{
   static bool nested;
   int rv = 0;

   // disallow nested calls
   //
   if (!nested)
   {
      nested = true;
      Console* con = Console::consoleList;
      while (con != 0)
      {
	 con->putString(string);
	 con = con->next;
      }
      nested = false;
   }

   while (*string)
   {
      SerialConsole::putChar(*string++);
      rv++;
   }

   return rv;
}
