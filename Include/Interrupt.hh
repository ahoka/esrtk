#ifndef INTERRUPT_HH
#define INTERRUPT_HH

#include <SystemTypes.hh>

// external interrupt handling
//

#include <cstdint>

class InterruptHandler
{
public:
   InterruptHandler();
   virtual ~InterruptHandler() = default;

   InterruptHandler(const InterruptHandler&) = delete;
   InterruptHandler& operator = (const InterruptHandler&) = delete;

   virtual void handleInterrupt() = 0;
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
   void handleInterrupt();

   uint64_t getCounter();
   void executeAllHandlers();

private:
   uint64_t counter;
};

class InterruptController;

class Interrupt
{
public:
   static bool registerHandler(irq_t irq, InterruptHandler* handler);
   static bool deregisterHandler(irq_t irq, InterruptHandler* handler);

   static bool enableInterrupt(irq_t irq);
   static bool disableInterrupt(irq_t irq);

   static int getInterruptLevel();

   static void setController(InterruptController* interruptController);
   static void handleInterrupt(irq_t irq);

   static void printStatistics();

private:
   static void appendHandler(InterruptHandler* h, InterruptHandler* handler);
   static void removeHandler(InterruptHandler* h, InterruptHandler* handler);

   enum
   {
      MaxInterrupts = 16
   };
   static DefaultInterruptHandler interruptHandlers[MaxInterrupts];

   static InterruptController* controller;

   friend class InterruptController;
};

#endif
