#include <Driver/Console.hh>

// XXX
#include <X86/SerialConsole.hh>
#include <StdioSupport.hh>

#include <functional>

namespace
{
   struct ConsoleList
   {
      Console* consoleM;
      ConsoleList* nextM;
      ConsoleList* prevM;
   };

   ConsoleList consoleList = { 0, &consoleList, &consoleList };

   void foreachConsole(std::function<void(Console*)> fun)
   {
      ConsoleList* c = consoleList.nextM;
      while (c != &consoleList)
      {
         fun(c->consoleM);
         c = c->nextM;
      }
   }
};

void
KernelStdio::registerConsole(Console* console)
{
   ConsoleList* item = new ConsoleList();
   item->consoleM = console;

   item->prevM = consoleList.prevM;
   consoleList.prevM->nextM = item;

   item->nextM = &consoleList;
   consoleList.prevM = item;
}

void
KernelStdio::unregisterConsole(Console *)
{
}

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
      foreachConsole([ch](Console* con) { con->putChar((char)ch); });
      // Console* con = Console::consoleList;
      // while (con != 0)
      // {
      //    con->putChar((char)ch);
      //    con = con->next;
      // }
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
      foreachConsole([string](Console* con) { con->putString(string); });
      nested = false;
   }

   while (*string)
   {
      SerialConsole::putChar(*string++);
      rv++;
   }

   return rv;
}
