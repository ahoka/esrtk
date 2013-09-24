#include <Interrupt.hh>
#include <Driver.hh>
#include <Debug.hh>

#include <X86/IoPort.hh>

#include <cstdio>

class Pit : InterruptHandler, Driver
{
public:
   Pit();
   ~Pit();

   int probe();
   bool init();
   bool finalize();

   void handler(irq_t irq);

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
Pit::handler(irq_t /*irq*/)
{
//   printf("PIT IRQ received\n");
}

int
Pit::probe()
{
   return 1;
}

bool
Pit::init()
{
   int hz = 100; // XXX
   int freq = OscillatorFrequency / hz;

   KASSERT((freq & ~0xffff) == 0);

   outb(Command, SelectChannel0 | LatchBothBytes | RateGeneratorMode);

   outb(Channel0, (uint8_t )freq);
   outb(Channel0, (uint8_t )(freq >> 8));

   Interrupt::registerHandler(0, this);
   Interrupt::enableInterrupt(0);

   return true;
}

bool
Pit::finalize()
{
   Interrupt::disableInterrupt(0);
   Interrupt::deregisterHandler(0, this);

   return true;
}

Pit pit;
