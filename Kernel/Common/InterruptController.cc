#include <InterruptController.hh>
#include <Interrupt.hh>
#include <cstdio>

InterruptController::InterruptController()
{
   printf("InterruptController()\n");
   Interrupt::setController(this);
}

InterruptController::~InterruptController()
{
   printf("~InterruptController()\n");
   Interrupt::setController(0);
}
