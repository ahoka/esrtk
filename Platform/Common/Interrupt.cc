#include <Debug.hh>
#include <Interrupt.hh>

#include <cstdio>

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
DefaultInterruptHandler::handler(irq_t /*irq*/)
{
   counter++;
}

uint64_t
DefaultInterruptHandler::getCounter()
{
   return counter;
}

void
DefaultInterruptHandler::executeAllHandlers(irq_t irq)
{
   InterruptHandler* handler = this;

   do
   {
      handler->handler(irq);
      handler = handler->getNext();
   }
   while (handler != this);
}

// Every interrupt has a default item, so we dont have to use a null value
// and have statistics easily
//
// TODO use a symbolic name
//
DefaultInterruptHandler Interrupt::interruptHandlers[MaxInterrupts];

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
Interrupt::registerHandler(InterruptHandler::irq_t irq, InterruptHandler* handler)
{
   if (irq >= MaxInterrupts)
   {
      return false;
   }

   appendHandler(&interruptHandlers[irq], handler);

   return true;
}

bool
Interrupt::deregisterHandler(InterruptHandler::irq_t irq, InterruptHandler* handler)
{
   if (irq >= MaxInterrupts)
   {
      return false;
   }

   removeHandler(&interruptHandlers[irq], handler);

   return false;
}

void
Interrupt::printStatistics()
{
   for (int i = 0; i < MaxInterrupts; i++)
   {
      printf("IRQ%d: %llu\n", i, (unsigned long long )interruptHandlers[i].getCounter());
   }
}
