#ifndef MEMORY_HH
#define MEMORY_HH

#include <Debug.hh>
#include <Parameters.hh>
#include <Platform/MemorySegment.hh>
#include <Platform/PhysicalPage.hh>
#include <Platform/PageCluster.hh>

#include <spinlock.h>

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

namespace Memory
{
   enum Flags
   {
      MapLazy = (1 << 0),
      MapUncacheable = (1 << 1),
      MapUser = (1 << 2)
   };

   void init();
   bool handlePageFault(uintptr_t address, InterruptFrame* frame);
   void copyMemoryMap();
   void addMemoryMapEntry(uintptr_t start, size_t length);

   uintptr_t getPage();
   void putPage(uintptr_t paddr);

   bool mapPage(uintptr_t vaddr, uintptr_t paddr, int flags = 0);
   uintptr_t mapAnonymousPage(uintptr_t paddr, int flags = 0);
   bool unmapPage(uintptr_t vaddr);

   uintptr_t mapRegion(uintptr_t paddr, size_t size, int flags = 0);
   bool unmapRegion(uintptr_t paddr, size_t size);
   uintptr_t mapAnonymousRegion(size_t size, int flags = 0);

   uintptr_t getPhysicalAddress(uintptr_t virt);

   void* readPhysicalMemory(void* destination, const void* source, size_t size);

   bool createKernelStack(uintptr_t& start);

   void info();
};

#endif
