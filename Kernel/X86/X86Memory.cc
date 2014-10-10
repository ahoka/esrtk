#include <Memory.hh>
#include <Multiboot.hh>

#include <X86/PageDirectory.hh>

#include <cstring>

using Multiboot::MultibootStructure;

// must be called when in 1:1 mapping
//
void
Memory::copyMemoryMap()
{
   const MultibootStructure* mb = Multiboot::getMultibootStructure();
   bool foundUsableMemory = false;

   // XXX does not belong here
   mb->print();

   printf("Parsing multiboot (%p) memory map: 0x%0x-0x%0x\n", mb,
	  mb->memoryMapAddress,
	  mb->memoryMapAddress + mb->memoryMapLength);

   for (MultibootStructure::MemoryMap *map = (MultibootStructure::MemoryMap *)mb->memoryMapAddress;
        map < (MultibootStructure::MemoryMap *)(mb->memoryMapAddress + mb->memoryMapLength);
        map++)
   {
      if (map->type == MultibootStructure::MemoryMap::Available)
      {
         printf("Usable memory at %p-%p\n", (void *)map->address, (void *)(map->address + map->length));
         if (map->address + map->length <= 0x100000)
         {
            // dont add low mem to the pool
            // we need it for special porpuses, like startup code for APs
            printf("Skipping Low Memory...\n");
         }
         else
         {
            addMemoryMapEntry(map->address, map->length);
            foundUsableMemory = true;
         }
      }
      else if (map->type == MultibootStructure::MemoryMap::Reserved)
      {
         printf("Reserved memory at %p-%p\n", (void *)map->address, (void *)(map->address + map->length));
      }
      else if (map->type == MultibootStructure::MemoryMap::Acpi)
      {
         printf("ACPI memory at %p-%p\n", (void *)map->address, (void *)(map->address + map->length));
      }
      else if (map->type == MultibootStructure::MemoryMap::Nvs)
      {
         printf("NVS memory at %p-%p\n", (void *)map->address, (void *)(map->address + map->length));
      }
   }

   printf("Finished copying memory map\n");

   KASSERT(foundUsableMemory);
}

bool
Memory::handlePageFault(uintptr_t /*address*/, InterruptFrame* /*frame*/)
{
#ifdef HAS_HEAP
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
#endif

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

bool
Memory::mapPage(uintptr_t virt, uintptr_t phys, int flags)
{
   return PageDirectory::mapPage(virt, phys, flags);
}

bool
Memory::unmapPage(uintptr_t virt)
{
   return PageDirectory::unmapPage(virt);
}

uintptr_t
Memory::getPhysicalAddress(uintptr_t virt)
{
   return PageDirectory::getPhysicalPage(virt);
}
