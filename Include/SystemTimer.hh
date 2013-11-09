#ifndef SYSTEMTIMER_HH
#define SYSTEMTIMER_HH

#include <cstdint>

class SystemTimer
{
public:
   SystemTimer();
   virtual ~SystemTimer();

   virtual int probe() = 0;
   virtual bool startTimer() = 0;
   virtual bool stopTimer() = 0;
   virtual unsigned int getFrequency() = 0;
   virtual void delay(unsigned long ms) = 0;

protected:
   void tick();

private:
   static SystemTimer* timers;
   SystemTimer* nextTimer;

   friend class Platform;
   static void probeAndInit();
};

#endif

