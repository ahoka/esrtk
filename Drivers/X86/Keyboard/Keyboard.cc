#include <X86/IoPort.hh>

#include <Debug.hh>
#include <cstdio>
#include <cstdint>

#include "Keyboard.hh"
#include "Scancodes.hh"

Keyboard::Keyboard()
{
   printf("Keyboard::Keyboard()\n");
   //   init();
}

Keyboard::~Keyboard()
{
   printf("Keyboard::~Keyboard()\n");
   //   finalize();
}

int
Keyboard::probe()
{
   return 1;
}

bool
Keyboard::init()
{
   bool success = Interrupt::registerHandler(1, this);

   return success;
}

bool
Keyboard::finalize()
{
   bool success = Interrupt::deregisterHandler(1, this);

   return success;
}

void
Keyboard::handler(irq_t /*irq*/)
{
   uint8_t scanCode = inb(0x60);

   // ignore break
   if (scanCode >= 0x80)
   {
      return;
   }

   Scancodes::Codes code = Scancodes::codes[scanCode];

   if (!code.isMeta)
   {
      putchar(code.ascii);
   }
   else
   {
      switch (code.ascii)
      {
         case Scancodes::Enter:
            putchar('\n');
            break;
         default:
            printf("0x%hhx", scanCode);
      }
   }
}

Keyboard keyboard;
