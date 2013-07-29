#ifndef MEMORY_HH
#define MEMORY_HH

#include <Multiboot.hh>

#include <cstdint>

struct MemorySegment
{
   uint32_t address;
   uint32_t size;
   MemorySegment* next;
};

class Memory
{
public:
   static bool handlePageFault(uint32_t address);
   static void copyMultibootMap(Multiboot* mb);
   static void init();

private:
   Memory();

//   static uint32_t memoryMap;

//   static uint32_t physStart;
//   static uint32_t physEnd;

   static uint32_t heapEnd;
   static uint32_t stackEnd;
};

#endif
