#include <PageFrameAllocator.hh>
#include <X86/Parameters.hh>
#include <X86/Memory.hh>
#include <Multiboot.hh>

extern Multiboot* mbd;

uint32_t Memory::heapEnd = HeapStart;
uint32_t Memory::stackEnd = StackStart;

// must be called when in 1:1 mapping
//
void
Memory::init()
{
   copyMultibootMap(mbd);
}

bool
Memory::handlePageFault(uint32_t address)
{
   (void )address;

   return false;
}

void
Memory::copyMultibootMap(Multiboot* mb)
{
   for (Multiboot::MemoryMap *map = (Multiboot::MemoryMap *)mb->memoryMapAddress;
        map < (Multiboot::MemoryMap *)(mb->memoryMapAddress + mb->memoryMapLength);
        map++)
   {
      if (map->type == Multiboot::MemoryMap::Available)
      {
         printf("Usable memory at %p-%p\n", (void *)map->address, (void *)(map->address + map->length));
         // TODO actually store it
      }
   }
}
