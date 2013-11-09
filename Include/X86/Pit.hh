#ifndef PIT_HH
#define PIT_HH

#include <Interrupt.hh>
#include <SystemTimer.hh>

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

   static unsigned long readValue();
   void delay(unsigned long ms);
   static void msleep(unsigned long ms);

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

   static unsigned long divider;
};

#endif
