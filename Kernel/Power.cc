#include "Power.hh"

// empty

void
Power::onIdle(void *)
{
    for (;;)
    {
        Power::halt();
    }
}
