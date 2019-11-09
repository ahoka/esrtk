#include <X86/IoPort.hh>
#include <StdioSupport.hh>

#include <Debug.hh>
#include <cstdio>
#include <cstdint>
#include <cctype>

#include "Keyboard.hh"
#include "Scancodes.hh"

Keyboard::Keyboard()
   : shiftPressed(false),
     controlPressed(false)
{
   Debug::info("Keyboard::Keyboard()\n");
}

Keyboard::~Keyboard()
{
   Debug::info("Keyboard::~Keyboard()\n");
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
   Interrupt::enableInterrupt(1);

   driverInfo("initalized\n");

   return success;
}

bool
Keyboard::finalize()
{
   Interrupt::disableInterrupt(1);
   bool success = Interrupt::deregisterHandler(1, this);

   return success;
}

const char*
Keyboard::name() const
{
   return "keyboard";
}

void
Keyboard::handleInterrupt()
{
   // check if buffer is ready
   uint8_t status = inb(0x64);
   if ((status & 0x1) == 0) {
      return;
   }
   
   uint8_t scanCode = inb(0x60);

   // ignore break
   bool isBreak = false;
   if (scanCode & 0x80)
   {
      isBreak = true;
      scanCode ^= 0x80;
   }

   Scancodes::Codes code = Scancodes::codes[scanCode];

   // For Kernel debug. This should be a cleaner interface.
   if (code.type == Scancodes::Special && code.ascii == Scancodes::F10)
   {
      Debug::panic("Kernel panic was requested from keyboard.\n");
   }

   if (code.type == Scancodes::Regular && isBreak)
   {
      return;
   }

   if (code.type == Scancodes::Regular)
   {
      if (shiftPressed)
      {
         console_feed(code.altascii);
      }
      else
      {
         console_feed(code.ascii);
      }
   }
   else
   {
      switch (code.ascii)
      {
         case Scancodes::Enter:
            console_feed('\n');
            break;
         case Scancodes::Space:
            console_feed(' ');
            break;
         case Scancodes::Tab:
            console_feed('\t');
            break;
         case Scancodes::LeftShift:
         case Scancodes::RightShift:
            shiftPressed = !isBreak;
            break;
         case Scancodes::Control:
            controlPressed = !isBreak;
            break;

         default:
            printf("0x%hhx", scanCode);
      }
   }
}

static Keyboard keyboard;
