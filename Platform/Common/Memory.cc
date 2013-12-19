#include <PageFrameAllocator.hh>
#include <Parameters.hh>
#include <Memory.hh>
#include <Multiboot.hh>
#include <Debug.hh>
#include <Templates.hh>
#include <Modules.hh>
#include <Memory.hh>
#include <Hal.hh>

#include <cstring>

//extern uintptr_t initial_stack;

// used in Multiboot.S
extern uintptr_t thread_zero_stack;

uintptr_t Memory::heapEnd = HeapStart;
uintptr_t Memory::stackEnd = StackStart;
uintptr_t Memory::mapEnd = MapStart;

MemorySegment Memory::memoryMap[MemoryMapMax];
unsigned int Memory::memoryMapCount = 0;

PageCluster Memory::usedPages;
PageCluster Memory::freePages;

//#define DEBUG

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

   uintptr_t firstUsableMemory = max((uintptr_t )&__end_kernel, __end_modules);
   uintptr_t firstFreePage = roundTo<uintptr_t>(firstUsableMemory, PageSize) + PageSize * PageFrameCount;

   // init page clusters
   PhysicalPage* bootstrapPage = (PhysicalPage* )firstFreePage;

   bootstrapPage->setAddress((uintptr_t )bootstrapPage - KernelVirtualBase);
   printf("Inserting bootstrap page to used pages cluster: %p\n", (void* )bootstrapPage->getAddress());
   usedPages.insert(bootstrapPage);
   
   unsigned int freeStructures = PageSize / sizeof (PhysicalPage) - 1;

   KASSERT(freeStructures > 0);
   printf("Free structures: %u\n", freeStructures);

   PhysicalPage* p = bootstrapPage + 1;
   for (unsigned int i = 0; i < memoryMapCount; i++)
   {
      printf("Adding memory range from %p-%p\n",
	     (void* )memoryMap[i].address,
	     (void* )(memoryMap[i].address + memoryMap[i].size));

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

	 p->setAddress(addr);
	 if ((addr >= KernelLoadAddress) && (addr < firstFreePage - KernelVirtualBase))
	 {
#ifdef DEBUG
	    printf("Found used page: %p\n", (void* )addr);
#endif
	    usedPages.insert(p);
	 }
	 else
	 {
	    freePages.insert(p);
	 }

	 p++;
	 freeStructures--;
      }
   }

   // map memory for the thread0 kernel stack
   //
//   bool success = createKernelStack(thread_zero_stack);

   uintptr_t start = stackEnd;

   printf("Creating initial kernel stack: %p-%p (%u)\n", (void* )start, (void* )(start - StackSize), StackSize);

   for (uintptr_t stackAddress = (start - StackSize);
	stackAddress < start;
	stackAddress += PageSize)
   {
      uintptr_t stackPage = getPage();
      KASSERT(stackPage != 0);

      bool success = mapPage(stackAddress, stackPage);
      KASSERT(success);
   }

   // add one unmapped page as guard
   stackEnd = start - StackSize - PageSize;

//   KASSERT(success);

//    printf("Freeing initial kernel stack: %p-%p\n", (void* )(initial_stack - InitialStackSize), (void* )initial_stack);
//    for (uintptr_t oldStack = initial_stack - InitialStackSize;
// 	oldStack < initial_stack;
// 	oldStack += PageSize)
//    {
//      unmap(oldStack);
//    }
}

// XXX curently this overlaps anon mappings, se we can only give one anon
// mapped page as the kernel stack area
#if 0
bool
Memory::createKernelStack(uintptr_t& start)
{
   start = stackEnd;

   printf("Creating new kernel stack: %p-%p (%u)\n", (void* )start, (void* )(start - StackSize), StackSize);

   for (uintptr_t stackAddress = (start - StackSize);
	stackAddress < start;
	stackAddress += PageSize)
   {
      uintptr_t stackPage = getPage();
      KASSERT(stackPage != 0);

      bool success = mapPage(stackAddress, stackPage);
      KASSERT(success);
   }

   // add one unmapped page as guard
   stackEnd = start - StackSize - PageSize;

   return true;
}
#else
bool
Memory::createKernelStack(uintptr_t& top)
{
   uintptr_t stackPage = getPage();
   KASSERT(stackPage != 0);

   uintptr_t bottom = mapPage(stackPage);
   KASSERT(bottom != 0);

   top = bottom + PageSize;

   printf("Creating new kernel stack: %p-%p (%u)\n", (void* )top, (void* )(bottom), PageSize);

   uintptr_t newStack = Hal::initKernelStack(top);

   printf("Stack after init: %p\n", (void*)newStack);

   top = newStack;

   return true;
}
#endif

bool
Memory::mapPage(uintptr_t address, uintptr_t phys)
{
   return Hal::mapPage(address, phys);
}

bool
Memory::unmapPage(uintptr_t page)
{
   bool success = Hal::unmapPage(page);
   // TODO we need to know where to mark the vaddr free!

   return success;
}

// anonymous mapping of a physical page
//
uintptr_t
Memory::mapPage(uintptr_t phys)
{
   mapEnd -= PageSize;

   if (mapEnd <= heapEnd)
   {
      Debug::panic("Kernel map namespace exhausted.");
   }

//   Debug::info("Mapping anonymous page: %p to %p\n", phys, mapEnd);
   bool success = mapPage(mapEnd, phys);
   if (!success)
   {
      return 0;
   }

   return mapEnd;
}

// anonymous mapping of a memory region
//
uintptr_t Memory::mapRegion(uintptr_t paddr, std::size_t size)
{
   std::size_t rsize = roundTo<uintptr_t>(size, PageSize);
   mapEnd -= rsize;

   uintptr_t firstPage = roundDown(paddr, PageSize);
   uintptr_t offset = paddr - firstPage;

   if (mapEnd <= heapEnd)
   {
      Debug::panic("Kernel map namespace exhausted.");
   }

   uintptr_t vaddr = mapEnd;
   for (uintptr_t page = firstPage;
	page < firstPage + rsize;
	page += PageSize, vaddr += PageSize)
   {
      bool success = mapPage(vaddr, page);
      if (!success)
      {
	 Debug::panic("mapRegion unsuccesful.");
	 // XXX unmap and return error
	 //return 0;
      }
   }

   return mapEnd + offset;
}

bool
Memory::unmapRegion(uintptr_t paddr, std::size_t size)
{
   std::size_t rsize = roundTo<uintptr_t>(size, PageSize);
   uintptr_t firstPage = roundDown(paddr, PageSize);

   for (uintptr_t page = firstPage;
	page < firstPage + rsize;
	page += PageSize)
   {
      bool success = unmapPage(page);
      if (!success)
      {
	 Debug::panic("unmapRegion unsuccesful.");
      }
   }

   return true;
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

   if (heapEnd >= mapEnd)
   {
      Debug::panic("Kernel heap exhausted.");
   }

#ifdef DEBUG
   printf("Heap sbrk: %p -> %p (%u)\n", (void *)oldEnd, (void *)heapEnd, heapEnd - oldEnd);
#endif

   return oldEnd;
}

// get a free physical page
//
uintptr_t
Memory::getPage()
{
   PhysicalPage* page = freePages.get();

   if (page != 0)
   {
      usedPages.insert(page);
      return page->getAddress();
   }

   return 0;
}

// free a physical page
//
void
Memory::putPage(uintptr_t address)
{
   PhysicalPage* page = usedPages.find(address);

   if (page == 0)
   {
      Debug::panic("Trying to free an unallocated physical page: %p\n", (void* )address);
   }

   usedPages.remove(page);
   freePages.insert(page);
}

void*
Memory::readPhysicalMemory(void* destination, const void* source, std::size_t size)
{
   uintptr_t mapFirst = roundDown(uintptr_t(source), PageSize);
   uintptr_t mapLast = roundTo(uintptr_t(source), PageSize);

   uintptr_t mapped = mapRegion(mapFirst, mapLast - mapFirst);

   std::memcpy(destination, reinterpret_cast<void*>(mapped + (uintptr_t(source) - mapFirst)), size);

   unmapRegion(mapped, mapLast - mapFirst);

   return destination;
}
