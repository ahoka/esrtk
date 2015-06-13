#include <Driver/Console.hh>

// XXX
#include <X86/EarlySerial.hh>
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
   volatile size_t consoleBufferPosition = 0;
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
      // local echo
      console_putchar(c);

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
   }

   return consoleBuffer[--consoleBufferPosition];
}

int
console_putchar(int ch)
{
   static bool nested;

   // disallow nested calls
   //
   if (!nested)
   {
      nested = true;

      EarlySerial::putChar(ch);
      foreachConsole([ch](Console* con)
      {
         if (con->isEnabled())
         {
            con->putChar((char)ch);
         }
      });

      nested = false;
   }

   return ch;
}

int
console_puts(const char* string)
{
   while (*string)
   {
      console_putchar(*string++);
   }

   return 1;
}
