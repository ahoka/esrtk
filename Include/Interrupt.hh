#ifndef INTERRUPT_HH
#define INTERRUPT_HH

// external interrupt handling
//

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
class DefaultInterruptHandler : public InterruptHandler
{
public:
   DefaultInterruptHandler();
   void handler(irq_t irq);

   uint64_t getCounter();
   void executeAllHandlers(irq_t irq);

private:
   uint64_t counter;
};

class Interrupt
{
public:
   static bool registerHandler(InterruptHandler::irq_t irq, InterruptHandler* handler);
   static bool deregisterHandler(InterruptHandler::irq_t irq, InterruptHandler* handler);

   static void printStatistics();

private:
   static void appendHandler(InterruptHandler* h, InterruptHandler* handler);
   static void removeHandler(InterruptHandler* h, InterruptHandler* handler);

   enum
   {
      MaxInterrupts = 16
   };
   static DefaultInterruptHandler interruptHandlers[MaxInterrupts];
};

#endif
