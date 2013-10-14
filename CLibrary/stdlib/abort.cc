#include <stdlib.h>
#include <Debug.hh>

void
abort()
{
   Debug::panic("Aborted!");
}
