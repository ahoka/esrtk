#include <Interrupt.hh>
#include <SystemTimer.hh>
#include <Debug.hh>

#include <X86/TimeStampCounter.hh>

#include <X86/IoPort.hh>

#include <cstdio>

class Pit : InterruptHandler, SystemTimer
{
public:
   Pit();
   ~Pit();

   int probe();
   bool startTimer();
   bool stopTimer();
   unsigned int getFrequency();

   void handleInterrupt();

private:
   enum Ports
   {
      Channel0 = 0x40,
      Channel1 = 0x41,
      Channel2 = 0x42,
      Command = 0x43
   };

   enum
   {
      OscillatorFrequency = 1193182
   };

   enum Command
   {
      SelectChannel0 = 0x0 << 6,
      SelectChannel1 = 0x1 << 6,
      SelectChannel2 = 0x2 << 6,
      ReadBack = 0x3 << 6,
      LatchCountValue = 0x0 << 4,
      LatchLowByteOnly = 0x1 << 4,
      LatchHighByteOnly = 0x2 << 4,
      LatchBothBytes = 0x3 << 4,
      InterruptOnTerminalCountMode = 0x0 << 1,
      SquareWaveMode = 0x1 << 1,
      RateGeneratorMode = 0x2 << 1,
      BcdMode = 0x1 << 0
   };
};

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

   int divider = OscillatorFrequency / getFrequency();

   KASSERT((divider & ~0xffff) == 0);

   outb(Command, SelectChannel0 | LatchBothBytes | RateGeneratorMode);

   outb(Channel0, (uint8_t )divider);
   outb(Channel0, (uint8_t )(divider >> 8));

   Interrupt::registerHandler(0, this);
   Interrupt::enableInterrupt(0);

   //
   printf("Calibrating TSC\n");
   TimeStampCounter::calibrate();
   printf("Frequency: %lu\n", TimeStampCounter::getFrequency());
   //

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

Pit pit;
