#ifndef TIMESTAMPCOUNTER_HH
#define TIMESTAMPCOUNTER_HH

#include <cstdint>

class TimeStampCounter
{
public:
   static void calibrate();
   static unsigned long getFrequency();
   static uint64_t readTsc();

private:
   static unsigned long frequency;

   TimeStampCounter();
   TimeStampCounter(const TimeStampCounter& orig);
   ~TimeStampCounter();
};

#endif
