#ifndef MEMORY_HH
#define MEMORY_HH

#include <Multiboot.hh>
#include <X86/Idt.hh>

#include <cstdint>
#include <cstddef>

extern void* __start_text;
extern void* __end_text;
extern void* __start_rodata;
extern void* __end_rodata;
extern void* __start_data;
extern void* __end_data;
extern void* __start_bss;
extern void* __end_bss;
extern void* __end_kernel;

struct MemorySegment
{
   uintptr_t address;
   std::size_t size;
   uint8_t* bitmap;

   MemorySegment() :
      address(0),
      size(0),
      bitmap(0)
   {
   }
};

enum
{
   MemoryMapMax = 16,
   PhysMapSize = 128 * 1024
};

class Memory
{
public:
   static bool handlePageFault(uint32_t address, InterruptFrame* frame);
   static void copyMultibootMap(Multiboot* mb);
   static void init();
   static uintptr_t sbrk(std::size_t size);
   static uintptr_t getFreePage();
   static bool map(uintptr_t virt, uintptr_t phys);

private:
   Memory();

   static uint32_t heapEnd;
   static uint32_t stackEnd;
   static uint32_t nextFreePage;

   static MemorySegment memoryMap[16];
   static int memoryMapCount;
   
   static uint8_t physMap[PhysMapSize];
};

#endif
