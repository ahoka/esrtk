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

//uint8_t Memory::physMap[PhysMapSize];

PageCluster Memory::usedPages;
PageCluster Memory::freePages;

// must be called when in 1:1 mapping
//
void
Memory::init()
{
   // for (std::size_t i = 0; i < sizeof physMap; i++)
   // {
   //    physMap[i] = 0xff;
   // }

   printf("\n\nPage size: %u, PhysicalPage structure size: %u\n", PageSize, sizeof(PhysicalPage));

   usedPages.init();
   freePages.init();

   uintptr_t nextFreePage = roundTo<uintptr_t>((uintptr_t )&__end_kernel, PageSize) + 4096;

   // init page clusters
   PhysicalPage* bootstrapPage = (PhysicalPage* )nextFreePage;

   bootstrapPage->address = (uintptr_t )bootstrapPage - KernelVirtualBase;
   printf("Inserting bootstrap page to used pages cluster: %p\n", (void* )bootstrapPage->address);
   usedPages.insert(bootstrapPage);
   
   unsigned int freeStructures = PageSize / sizeof (PhysicalPage) - 1;

   KASSERT(freeStructures > 0);
   printf("Free structures: %u\n", freeStructures);

   printf("Listing Kernel sections:\n");
   printf(".text: %p-%p\n", &__start_text, &__end_text);
   printf(".data: %p-%p\n", &__start_data, &__end_data);
   printf(".rodata: %p-%p\n", &__start_rodata, &__end_rodata);
   printf(".bss: %p-%p\n", &__start_bss, &__end_bss);

   mbd->print();

   copyMultibootMap(mbd);

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
	    printf("Allocating new page structure\n");
	    p = freePages.get();
	    KASSERT(p != 0);
	    usedPages.insert(p);

	    freeStructures = PageSize / sizeof (PhysicalPage);
	 }

//	 printf("p: 0x%lx\n", (unsigned long )p);
	 p->address = addr;
	 freePages.insert(p);
	 p++;
      }
   }
}

bool Memory::map(uintptr_t address, uintptr_t phys)
{
   return PageDirectory::mapPage(address, phys);
}

bool
Memory::handlePageFault(uintptr_t address, InterruptFrame* frame)
{
   if (address >= HeapStart && address < heapEnd)
   {
      uintptr_t pageAddress = address & PageMask;

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
      uintptr_t pageAddress = address & PageMask;

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

   KASSERT((heapEnd & PageSize) == 0);

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
