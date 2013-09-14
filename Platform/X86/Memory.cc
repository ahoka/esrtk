#include <PageFrameAllocator.hh>
#include <X86/Parameters.hh>
#include <X86/Memory.hh>
#include <Multiboot.hh>
#include <Debug.hh>
#include <Templates.hh>
#include <X86/Idt.hh>
#include <X86/PageDirectory.hh>

extern Multiboot* mbd;

uintptr_t Memory::heapEnd = HeapStart;
uintptr_t Memory::stackEnd = StackStart;

MemorySegment Memory::memoryMap[MemoryMapMax];
unsigned int Memory::memoryMapCount = 0;

PageCluster Memory::usedPages;
PageCluster Memory::freePages;

void
Memory::init()
{
   printf("Listing Kernel sections:\n");
   printf(".text: %p-%p\n", &__start_text, &__end_text);
   printf(".data: %p-%p\n", &__start_data, &__end_data);
   printf(".rodata: %p-%p\n", &__start_rodata, &__end_rodata);
   printf(".bss: %p-%p\n", &__start_bss, &__end_bss);

   printf("Page size: %u, PhysicalPage structure size: %u\n", PageSize, sizeof(PhysicalPage));

   usedPages.init();
   freePages.init();

   uintptr_t nextFreePage = roundTo<uintptr_t>((uintptr_t )&__end_kernel, PageSize) + PageSize * PageFrameCount;

   // init page clusters
   PhysicalPage* bootstrapPage = (PhysicalPage* )nextFreePage;

   bootstrapPage->address = (uintptr_t )bootstrapPage - KernelVirtualBase;
   printf("Inserting bootstrap page to used pages cluster: %p\n", (void* )bootstrapPage->address);
   usedPages.insert(bootstrapPage);
   
   unsigned int freeStructures = PageSize / sizeof (PhysicalPage) - 1;

   KASSERT(freeStructures > 0);
   printf("Free structures: %u\n", freeStructures);

   PhysicalPage* p = bootstrapPage + 1;
   for (unsigned int i = 0; i < memoryMapCount; i++)
   {
      printf("Adding memory range from %lu - %lu\n",
	     (unsigned long )memoryMap[i].address,
	     (unsigned long )memoryMap[i].address + memoryMap[i].size);

      for (uintptr_t addr = memoryMap[i].address;
	   addr < memoryMap[i].address + memoryMap[i].size;
	   addr += PageSize)
      {
	 if (freeStructures == 0)
	 {
#ifdef DEBUG
	    printf("Allocating new page structure\n");
#endif
	    p = (PhysicalPage* )sbrk(PageSize);
	    KASSERT(p != 0);

	    freeStructures = PageSize / sizeof (PhysicalPage);
	 }

	 p->address = addr;
	 freePages.insert(p);
	 p++;
	 freeStructures--;
      }
   }
}

bool Memory::map(uintptr_t address, uintptr_t phys)
{
#ifdef DEBUG
   printf("Mapping physical page %p to %p\n", (void *)phys, (void *)address);
#endif
   
   return PageDirectory::mapPage(address, phys);
}

bool
Memory::handlePageFault(uintptr_t address, InterruptFrame* frame)
{
   if (address >= HeapStart && address < heapEnd)
   {
      uintptr_t pageAddress = address & ~PageMask;
#ifdef DEBUG
      printf("Expanding kernel heap: %p\n", (void* )pageAddress);
#endif
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
      uintptr_t pageAddress = address & ~PageMask;

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
   uintptr_t oldEnd = heapEnd;

   // align size with PageSize
   //
   heapEnd += roundTo<std::size_t>(size, PageSize);

   KASSERT((heapEnd & PageMask) == 0);

   return oldEnd;
}

// map a physical page
//
uintptr_t
Memory::getFreePage()
{
   PhysicalPage* page = freePages.get();

   if (page != 0)
   {
      usedPages.insert(page);
   }

   return page->address;
}

// must be called when in 1:1 mapping
//
void
Memory::copyMemoryMap()
{
   Multiboot* mb = mbd;

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
