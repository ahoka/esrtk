#ifndef MEMORY_HH
#define MEMORY_HH

#include <cstdint>

class Memory
{
public:
   static bool handlePageFault(uint32_t address);
   static void init();

private:
   Memory();

   static uint32_t physStart;
   static uint32_t physEnd;

   static uint32_t heapEnd;
   static uint32_t stackEnd;
};

#endif
