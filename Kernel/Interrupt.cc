#include <Debug.hh>
#include <Interrupt.hh>
#include <InterruptController.hh>

#include <cstdio>
#include <cstdint>

// interrupt handler collecting statistics
namespace {
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
}

InterruptController* Interrupt::controller = 0;

extern int interrupt_level;
int interrupt_level = 0;

void
InterruptHandler::setNext(InterruptHandler* value)
{
   next = value;
}

InterruptHandler*
InterruptHandler::getNext()
{
   return next;
}

DefaultInterruptHandler::DefaultInterruptHandler() :
   counter(0)
{
   setNext(this);
}

void
DefaultInterruptHandler::handleInterrupt()
{
   counter++;
}

uint64_t
DefaultInterruptHandler::getCounter()
{
   return counter;
}

void
DefaultInterruptHandler::executeAllHandlers()
{
   InterruptHandler* handler = this;

   do
   {
      handler->handleInterrupt();
      handler = handler->getNext();
   }
   while (handler != this);
}

// Every interrupt has a default item, so we dont have to use a null value
// and have statistics easily
//
InterruptHandler*
Interrupt::getInterruptHandler(unsigned int interrupt)
{
   static DefaultInterruptHandler interruptHandlers[MaxInterrupts];

   KASSERT(interrupt < MaxInterrupts);

   return &interruptHandlers[interrupt];
}

void
Interrupt::appendHandler(InterruptHandler* h, InterruptHandler* handler)
{
   InterruptHandler *p = h;
   while (p->getNext() != h)
   {
      p = p->getNext();
   }

   InterruptHandler* oldNext = p->getNext();
   p->setNext(handler);
   handler->setNext(oldNext);
}

void
Interrupt::removeHandler(InterruptHandler* h, InterruptHandler* handler)
{
   InterruptHandler *p = h;
   InterruptHandler *prev = h;
   while (p->getNext() != h)
   {
      prev = p;
      p = p->getNext();
   }

   InterruptHandler* oldNext = p->getNext();
   prev->setNext(oldNext);

   handler->setNext(0);
}

bool
Interrupt::registerHandler(irq_t irq, InterruptHandler* handler)
{
   if (irq >= MaxInterrupts)
   {
      return false;
   }

   appendHandler(getInterruptHandler(irq), handler);

   return true;
}

bool
Interrupt::deregisterHandler(irq_t irq, InterruptHandler* handler)
{
   if (irq >= MaxInterrupts)
   {
      return false;
   }

   removeHandler(getInterruptHandler(irq), handler);

   return false;
}

bool
Interrupt::enableInterrupt(irq_t irq)
{
//   printf("Interrupt::enableInterrupt(%u)\n", irq);

   KASSERT(controller != 0);
   controller->enableInterrupt(irq);

   return true;
}

bool
Interrupt::disableInterrupt(irq_t irq)
{
//   printf("Interrupt::disableInterrupt(%u)\n", irq);

   KASSERT(controller != 0);
   controller->disableInterrupt(irq);

   return true;
}

void
Interrupt::printStatistics()
{
   for (int i = 0; i < MaxInterrupts; i++)
   {
      printf("IRQ%d:\t%llu\n", i, (unsigned long long)static_cast<DefaultInterruptHandler*>(getInterruptHandler(i))->getCounter());
   }
}

void
Interrupt::handleInterrupt(irq_t irq)
{
   KASSERT(irq < MaxInterrupts);

   static_cast<DefaultInterruptHandler*>(getInterruptHandler(irq))->executeAllHandlers();

   KASSERT(controller != 0);
   controller->endOfInterrupt(irq);
}

void
Interrupt::setController(InterruptController* interruptController)
{
   KASSERT((controller == 0 && interruptController != 0) || (interruptController == 0 && controller != 0));

   controller = interruptController;
}

int
Interrupt::getInterruptLevel()
{
   return interrupt_level;
}
