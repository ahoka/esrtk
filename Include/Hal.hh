#ifndef _HAL_HH_
#define _HAL_HH_

#include <cstdint>

class Hal
{
public:
   static bool mapPage(uintptr_t virt, uintptr_t phys);
   static bool unmapPage(uintptr_t virt);

// not really hal...
   static uintptr_t initKernelStack(uintptr_t stack);
};

#endif

