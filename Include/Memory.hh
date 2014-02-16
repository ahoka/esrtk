#ifndef MEMORY_HH
#define MEMORY_HH

#include <Debug.hh>
#include <Parameters.hh>
#include <Platform/MemorySegment.hh>
#include <Platform/PhysicalPage.hh>
#include <Platform/PageCluster.hh>

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

enum
{
   MemoryMapMax = 32,
   PhysMapSize = 128 * 1024
};

struct InterruptFrame;

class Memory
{
public:
   static void init();
   static bool handlePageFault(uintptr_t address, InterruptFrame* frame);
   static void copyMemoryMap();

   static uintptr_t getPage();
   static void putPage(uintptr_t paddr);
   static bool mapPage(uintptr_t vaddr, uintptr_t paddr);
   static uintptr_t mapPage(uintptr_t paddr);
   static bool unmapPage(uintptr_t vaddr);
   static uintptr_t mapRegion(uintptr_t paddr, std::size_t size);
   static bool unmapRegion(uintptr_t paddr, std::size_t size);
   static uintptr_t mapAnonymousRegion(std::size_t size);
   static void* readPhysicalMemory(void* destination, const void* source, std::size_t size);
   static bool createKernelStack(uintptr_t& start);

private:
   Memory() = delete;

   static uintptr_t heapEnd;
   static uintptr_t stackEnd;
   static uintptr_t mapEnd;

   static PageCluster usedPages;
   static PageCluster freePages;

   static MemorySegment memoryMap[MemoryMapMax];
   static unsigned int memoryMapCount;
};

#endif
