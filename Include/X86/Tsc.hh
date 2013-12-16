#ifndef TSC_HH
#define TSC_HH

#include <ClockProvider.hh>

#include <cstdint>

class Tsc : ClockProvider
{
public:
   Tsc();
   ~Tsc();

   int probe();
   bool startClock();
   bool stopClock();
   uint64_t getValue();
   unsigned int getFrequency();
   const char* getName();

private:
   void calibrate();
   uint64_t readTsc();

   unsigned long frequency;
   uint64_t offset;

   Tsc(const Tsc& orig) = delete;
};

#endif
