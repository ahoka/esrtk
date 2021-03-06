#include <Debug.hh>
#include <X86/IoPort.hh>
#include <X86/Pit.hh>

unsigned long Pit::divider = 0;

Pit::Pit()
{
   printf("Pit::Pit()\n");
}

Pit::~Pit()
{
   printf("Pit::~Pit()\n");
}

void
Pit::handleInterrupt()
{
   tick();
}

int
Pit::probe()
{
   return 1;
}

bool
Pit::startTimer()
{
   printf("Programmable interval timer starting\n");

   divider = OscillatorFrequency / getFrequency();

   KASSERT((divider & ~0xfffflu) == 0);

   outb(Command, SelectChannel0 | LatchBothBytes | RateGeneratorMode);
   outb(0, 0x80);

   outb(Channel0, (uint8_t )divider);
   outb(0, 0x80);
   outb(Channel0, (uint8_t )(divider >> 8));

   Interrupt::registerHandler(0, this);
   Interrupt::enableInterrupt(0);

   return true;
}

bool
Pit::stopTimer()
{
   Interrupt::disableInterrupt(0);
   Interrupt::deregisterHandler(0, this);

   return true;
}

unsigned int
Pit::getFrequency()
{
   return 100;
}

unsigned long
Pit::readValue()
{
   unsigned long low = inb(Channel0);
   unsigned long high = inb(Channel0);
   
   unsigned long value = low | high << 8;

   return value;
}

void
Pit::delay(unsigned long ms)
{
   // we depend on the pit being initialized for system timer
   //
   long ticks = (long)ms * (OscillatorFrequency / 1000);

   unsigned long lastTick = readValue();
   while (ticks > 1)
   {
      unsigned long currentTick = readValue();
      if (lastTick > currentTick)
      {
         ticks -= lastTick - currentTick;
      }

      lastTick = readValue();
   }
}

static Pit pit;

void
Pit::msleep(unsigned long ms)
{
   pit.delay(ms);
}
