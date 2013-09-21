#ifndef INTERRUPTCONTROLLER_HH
#define INTERRUPTCONTROLLER_HH

#include <SystemTypes.hh>
#include <cstdio>

class InterruptController
{
public:
   virtual void endOfInterrupt(irq_t irq) = 0;

   InterruptController();
   virtual ~InterruptController();
};

#endif

