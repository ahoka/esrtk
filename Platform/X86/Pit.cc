#include <Interrupt.hh>

#include <cstdio>

class Pit : InterruptHandler
{
public:
   Pit();
   ~Pit();

   void handler(irq_t irq);

private:
   enum Ports
   {
      Channel0 = 0x40,
      Channel1 = 0x41,
      Channel2 = 0x42,
      Command = 0x43
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
      InterruptOnTerminalCount = 0x0 << 1,
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
   printf("PIT IRQ received\n");
}

Pit pit;
