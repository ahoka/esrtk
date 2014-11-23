#include <Driver/Console.hh>

// XXX
#include <X86/SerialConsole.hh>
#include <StdioSupport.hh>
#include <Power.hh> // XXX

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

namespace
{
   const size_t consoleBufferSize = 4096;
   char consoleBuffer[consoleBufferSize] = { 0 };
   size_t consoleBufferPosition = 0;
};

int
console_feed(int c)
{
   if (consoleBufferPosition == consoleBufferSize)
   {
      printf("Console buffer overrun!\n");
      return 0;
   }
   else
   {
      consoleBuffer[consoleBufferPosition++] = c;
      return 1;
   }
}

int
console_getchar()
{
   // XXX sleep here!
   while (consoleBufferPosition == 0)
   {
      Power::halt();
   }

   return consoleBuffer[--consoleBufferPosition];
}

int
console_putchar(int ch)
{
   static bool nested;

   SerialConsole::putChar(ch);

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

   return ch;
}

int
console_puts(const char* string)
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
