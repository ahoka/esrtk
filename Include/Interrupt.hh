#ifndef INTERRUPT_HH
#define INTERRUPT_HH

#include <cstdint>

class InterruptHandler
{
public:
   typedef unsigned int irq_t;

   InterruptHandler() {}
   virtual ~InterruptHandler() {}

   virtual void handler(irq_t irq) = 0;
   void setNext(InterruptHandler* value);
   InterruptHandler* getNext();

private:
   InterruptHandler* next;
};

// interrupt handler collecting statistics
class DefaultInterruptHandler : InterruptHandler
{
public:
   DefaultInterruptHandler();
   void handler(irq_t irq);

   uint64_t getCounter();
   void executeAllHandlers(irq_t irq);

private:
   uint64_t counter;
};

#endif
