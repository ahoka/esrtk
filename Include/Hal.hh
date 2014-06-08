#ifndef _HAL_HH_
#define _HAL_HH_

#include <cstdint>

using InterruptFlags = uint32_t;

class Hal
{
public:
   static bool mapPage(uintptr_t virt, uintptr_t phys, int flags);
   static bool unmapPage(uintptr_t virt);
   static uintptr_t getPhysicalAddress(uintptr_t virt);

   static void disableLocalInterrupts();
   static void enableLocalInterrupts();
   static void saveLocalInterrupts(InterruptFlags& flags);
   static void restoreLocalInterrupts(InterruptFlags& flags);

// not really hal...
   static uintptr_t initKernelStack(uintptr_t stack);
};

#endif

