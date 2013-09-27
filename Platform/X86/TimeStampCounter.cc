#include <X86/Assembly.hh>
#include <X86/IoPort.hh>
#include <X86/Pit.hh>

#include <X86/TimeStampCounter.hh>

#include <cstdio>

unsigned long TimeStampCounter::frequency = 0;

void
TimeStampCounter::calibrate()
{
   cli();

   const int measurements = 5;

   unsigned long results[measurements];

   unsigned long delay = 50;

   for (auto i = 0; i < measurements; i++)
   {
      uint64_t tsc0 = rdtsc();
      Pit::delay(delay);
      uint64_t tsc1 = rdtsc();

      results[i] = (unsigned long)(tsc1 - tsc0) / delay;
      
      printf("Delta: %lu\n", (unsigned long )((tsc1 - tsc0) / delay));
   }

   unsigned long min = ~0u;
   unsigned long max = 0u;
   unsigned long sum = 0u;
   for (auto i = 0; i < measurements; i++)
   {
      if (results[i] < min)
      {
         min = results[i];
      }

      if (results[i] > max)
      {
         max = results[i];
      }

      sum += results[i];
   }

   sum -= min;
   sum -= max;

   unsigned long average = sum / (measurements - 2);

   printf("Average TSC frequency is: %lu\n", average);

   frequency = average;

#if 0
   uint8_t keyboard = inb(0x61);

   // Enable Channel 2, disable speaker
   outb(0x61, (keyboard & ~0x2) | 0x1);
   
   outb(0xb0, 0x43);

   unsigned int ms = 10u;
   unsigned int divisor = 1193182u / (1000u / ms);

   printf("%u\n", divisor);

   for (int i = 0; i < 10; i++)
   {
      outb(divisor & 0xff, 0x42);
      outb((divisor >> 8) & 0xff, 0x42);
      
      uint64_t tsc0 = rdtsc();

      unsigned long cycles = 0;

      while ((inb(0x61) & 0x20) == 0x00)
      {
         cycles++;
         // empty
      }
      
      uint64_t tsc1 = rdtsc();

      printf("Delta: %lu (%lu cycles)\n", (unsigned long )((tsc1 - tsc0) / 50), cycles);
      frequency = (unsigned long )((tsc1 - tsc0) / 50);
   }

   outb(keyboard, 0x61);

#endif

   sti();
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
