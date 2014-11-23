#include <X86/IoPort.hh>
#include <StdioSupport.hh>

#include <Debug.hh>
#include <cstdio>
#include <cstdint>

#include "Keyboard.hh"
#include "Scancodes.hh"

// XXX debug hack
#include <Time.hh>
#include <Power.hh>
#include <MemoryManager.hh>
#include <Kernel/Thread.hh>

Keyboard::Keyboard()
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
   uint8_t scanCode = inb(0x60);

   // ignore break
   if (scanCode >= 0x80)
   {
      return;
   }

   Scancodes::Codes code = Scancodes::codes[scanCode];

   if (!code.isMeta)
   {
      console_feed(code.ascii);
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
         case Scancodes::F4:
            putchar('\n');
            Kernel::Thread::printAll();
            break;
         case Scancodes::F9:
            Power::reboot();
            //break;
         case Scancodes::F8:
            printf("Simulating a dead-lock!\n");
            for (;;)
            {
               asm volatile("cli");
            }
            break;

         default:
            printf("0x%hhx", scanCode);
      }
   }
}

static Keyboard keyboard;
