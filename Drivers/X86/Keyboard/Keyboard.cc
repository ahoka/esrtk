#include <X86/IoPort.hh>

#include <Debug.hh>
#include <cstdio>
#include <cstdint>

#include "Keyboard.hh"

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

   printf("0x%hhx", scanCode);
}

//Keyboard keyboard;
