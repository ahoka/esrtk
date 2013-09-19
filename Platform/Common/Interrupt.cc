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
DefaultInterruptHandler interruptHandlers[255];
