#ifndef _HAL_HH_
#define _HAL_HH_

#include <cstdint>

using InterruptFlags = uint32_t;

class Hal
{
public:
   static void disableLocalInterrupts();
   static void enableLocalInterrupts();
   static void saveLocalInterrupts(InterruptFlags& flags);
   static void restoreLocalInterrupts(InterruptFlags& flags);
};

#endif

