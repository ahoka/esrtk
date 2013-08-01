#include <PageFrameAllocator.hh>
#include <X86/Parameters.hh>
#include <X86/Memory.hh>
#include <Multiboot.hh>
#include <Debug.hh>
#include <Templates.hh>
#include <X86/Idt.hh>

extern Multiboot* mbd;

uint32_t Memory::heapEnd = HeapStart;
uint32_t Memory::stackEnd = StackStart;

uint32_t Memory::nextFreePage = roundTo<uint32_t>((uint32_t )&__end_kernel, PageSize);

MemorySegment Memory::memoryMap[MemoryMapMax];
int Memory::memoryMapCount = 0;

uint8_t Memory::physMap[PhysMapSize];

// must be called when in 1:1 mapping
//
void
Memory::init()
{
   for (std::size_t i = 0; i < sizeof physMap; i++)
   {
      physMap[i] = 0xff;
   }

   printf("Listing Kernel sections:\n");
   printf(".text: %p-%p\n", &__start_text, &__end_text);
   printf(".data: %p-%p\n", &__start_data, &__end_data);
   printf(".rodata: %p-%p\n", &__start_rodata, &__end_rodata);
   printf(".bss: %p-%p\n", &__start_bss, &__end_bss);

   mbd->print();

   copyMultibootMap(mbd);
}

bool
Memory::handlePageFault(uint32_t address, InterruptFrame* frame)
{
   if (address >= HeapStart && address < heapEnd)
   {
      uint32_t pageAddress = address & PageMask;

      printf("Expanding kernel heap: %p\n", (void* )pageAddress);

      uintptr_t page = getFreePage();
      KASSERT(page != 0);

      bool rv = map(pageAddress, page);
      KASSERT(rv);

      return true;
   }
   
   if (address < StackStart &&
       address >= (StackStart - StackSize) &&
       (frame->esp + 32) > address)
   {
      uint32_t pageAddress = address & PageMask;

      printf("Expanding kernel stack: %p\n", (void* )pageAddress);

      uintptr_t page = getFreePage();
      KASSERT(page != 0);

      bool rv = map(pageAddress, page);
      KASSERT(rv);

      return true;
   }

   return false;
}

// increment heapEnd, the page fault handler will allocate it when accessed
//
uintptr_t
Memory::sbrk(std::size_t size)
{
   // align size with PageSize
   //
   heapEnd += roundTo<std::size_t>(size, PageSize);

   KASSERT((heapEnd & PageSize) == 0);
   return heapEnd;
}

// map a physical page
//
uintptr_t
Memory::getFreePage()
{
   return 0;
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
         KASSERT(memoryMapCount < MemoryMapMax);

         memoryMap[memoryMapCount].address = (uintptr_t )map->address;
         memoryMap[memoryMapCount].size = (std::size_t )map->length;
         memoryMapCount++;
      }
   }
}
