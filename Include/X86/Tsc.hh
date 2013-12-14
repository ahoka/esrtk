#ifndef TSC_HH
#define TSC_HH

#include <Clock.hh>

#include <cstdint>

class Tsc : Clock
{
public:
   Tsc();
   ~Tsc();

   int probe();
   bool startClock();
   bool stopClock();
   uint64_t getTime();
   const char* getName();

private:
   void calibrate();
   unsigned long getFrequency();
   uint64_t readTsc();

   unsigned long frequency;
   uint64_t offset;

   Tsc(const Tsc& orig);
};

#endif
