#ifndef CLOCKPROVIDER_HH
#define CLOCKPROVIDER_HH

#include <cstdint>

class ClockProvider
{
public:
   ClockProvider();
   virtual ~ClockProvider();

   virtual int probe() = 0;
   virtual bool startClock() = 0;
   virtual bool stopClock() = 0;
   virtual unsigned int getFrequency() = 0;
   virtual uint64_t getValue() = 0;
   virtual const char* getName() = 0;

private:
   friend class Clock;
   ClockProvider* nextClock;
};

#endif
