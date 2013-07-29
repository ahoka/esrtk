#ifndef MEMORY_HH
#define MEMORY_HH

#include <Multiboot.hh>

#include <cstdint>

extern void* __start_text;
extern void* __end_text;
extern void* __start_rodata;
extern void* __end_rodata;
extern void* __start_data;
extern void* __end_data;
extern void* __start_bss;
extern void* __end_bss;

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
