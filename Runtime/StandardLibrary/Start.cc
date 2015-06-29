#include "SystemCall.h"

extern "C" void
start()
{
   syscall(0, (void*)0x123456);

   for (;;)
   {
      asm volatile("pause");
   }
}
