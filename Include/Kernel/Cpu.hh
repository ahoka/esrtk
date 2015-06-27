#ifndef CPU_HH
#define CPU_HH

#include <cstdint>

namespace Cpu
{
   using InterruptFlags = uint32_t;

   void disableLocalInterrupts();
   void enableLocalInterrupts();
   void saveLocalInterrupts(InterruptFlags& flags);
   void restoreLocalInterrupts(InterruptFlags& flags);

   uint64_t getTimestamp();
}

#endif
