#include <X86/Assembly.hh>
#include <X86/IoPort.hh>

#include <X86/TimeStampCounter.hh>

unsigned long TimeStampCounter::frequency = 0;

void
TimeStampCounter::calibrate()
{
   uint8_t keyboard = inb(0x61);

   // Enable Channel 2, disable speaker
   outb(0x61, (keyboard & ~0x2) | 0x1);

   outb(0xb0, 0x43);

   unsigned int divisor = 1193182 / 1000 / 50;

   outb(divisor & 0xff, 0x42);
   outb(divisor >> 8, 0x42);

   uint64_t tsc0 = rdtsc();

   while ((inb(0x61) & 0x20) == 0x00)
   {
      // empty
   }

   uint64_t tsc1 = rdtsc();

   frequency = (unsigned long )((tsc1 - tsc0) / 50);

   outb(keyboard, 0x61);
}

unsigned long
TimeStampCounter::getFrequency()
{
   return frequency;
}

uint64_t
TimeStampCounter::readTsc()
{
   return rdtsc();
}
