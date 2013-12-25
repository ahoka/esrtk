#include <Memory.hh>
#include <Multiboot.hh>

#include <cstring>

extern Multiboot* mbd;

// must be called when in 1:1 mapping
//
void
Memory::copyMemoryMap()
{
   Multiboot* mb = mbd;
   bool foundUsableMemory = false;

   printf("Parsing multiboot (%p) memory map: 0x%0x-0x%0x\n", mbd,
	  mb->memoryMapAddress,
	  mb->memoryMapAddress + mb->memoryMapLength);

   for (Multiboot::MemoryMap *map = (Multiboot::MemoryMap *)mb->memoryMapAddress;
        map < (Multiboot::MemoryMap *)(mb->memoryMapAddress + mb->memoryMapLength);
        map++)
   {
      if (map->type == Multiboot::MemoryMap::Available)
      {
         printf("Usable memory at %p-%p\n", (void *)map->address, (void *)(map->address + map->length));
         KASSERT(memoryMapCount < MemoryMapMax);

         memoryMap[memoryMapCount].address = (uintptr_t )map->address;
         memoryMap[memoryMapCount].size = (std::size_t )map->length;
         memoryMapCount++;

	 foundUsableMemory = true;
      }
   }

   printf("Finished copying memory map\n");

   KASSERT(foundUsableMemory);
}


bool
Memory::handlePageFault(uintptr_t address, InterruptFrame* /*frame*/)
{
   if (address >= HeapStart && address < heapEnd)
   {
      uintptr_t pageAddress = address & ~PageMask;
#ifdef DEBUG
      printf("Expanding kernel heap: %p initiated by access to %p\n",
	     (void* )pageAddress, (void *)address);
#endif
      uintptr_t page = getPage();
      KASSERT(page != 0);

      bool rv = mapPage(pageAddress, page);
      KASSERT(rv);

      std::memset((void* )pageAddress, 0, PageSize);

      return true;
   }

#if 0
   // this is an instant triple fault
   if (address < StackStart &&
       address >= (StackStart - StackSize) &&
       (frame->esp + 32) > address)
   {
      uintptr_t pageAddress = address & ~PageMask;

      printf("Expanding kernel stack: %p\n", (void* )pageAddress);

      uintptr_t page = getPage();
      KASSERT(page != 0);

      bool rv = map(pageAddress, page);
      KASSERT(rv);

      return true;
   }
#endif

   return false;
}
