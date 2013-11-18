#include <X86/IoPort.hh>

#include <Debug.hh>
#include <cstdio>
#include <cstdint>

#include "Keyboard.hh"
#include "Scancodes.hh"

// XXX debug hack
#include <Time.hh>
#include <Power.hh>
#include <MemoryManager.hh>

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
   Interrupt::enableInterrupt(1);

   return success;
}

bool
Keyboard::finalize()
{
   Interrupt::disableInterrupt(1);
   bool success = Interrupt::deregisterHandler(1, this);

   return success;
}

void
Keyboard::handleInterrupt()
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
         case Scancodes::Space:
            putchar(' ');
            break;
         case Scancodes::Tab:
            putchar('\t');
            break;
         case Scancodes::F1:
            printf("Interrupt statistics:\n");
            Interrupt::printStatistics();
            break;
         case Scancodes::F2:
         {
            uint64_t uptime = Time::getUptime();
//            printf("\nUptime is: %lu\n", (unsigned long )uptime);
            printf("Uptime is: %lu.%lu\n",
                   (unsigned long )uptime / 1000,
                   (unsigned long )(uptime % 1000));
            break;
         }
         case Scancodes::F3:
            putchar('\n');
            MemoryManager::get().printStatistics();
            break;
         case Scancodes::F9:
            Power::reboot();
            //break;
         default:
            printf("0x%hhx", scanCode);
      }
   }
}

static Keyboard keyboard;
