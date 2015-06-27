#ifndef INTERRUPT_HH
#define INTERRUPT_HH

#include <SystemTypes.hh>
#include <Kernel/InterruptHandler.hh>

// external interrupt handling
//

class InterruptController;

class Interrupt
{
public:
   static bool registerHandler(irq_t irq, InterruptHandler* handler);
   static bool deregisterHandler(irq_t irq, InterruptHandler* handler);

   static bool enableInterrupt(irq_t irq);
   static bool disableInterrupt(irq_t irq);

   static int getInterruptLevel();

   static void printStatistics();

   // XXX should not be accessed globally
   static void handleInterrupt(irq_t irq);

private:
   static void appendHandler(InterruptHandler* h, InterruptHandler* handler);
   static void removeHandler(InterruptHandler* h, InterruptHandler* handler);

   static void setController(InterruptController* interruptController);

   enum
   {
      MaxInterrupts = 16
   };

   static InterruptController* controller;
   static InterruptHandler* getInterruptHandler(unsigned int interrupt);

   friend class InterruptController;
};

class InterruptLevel
{
public:
   enum
   {
      None = 0,
      SoftInterrupt = 1
   };
};

#endif
