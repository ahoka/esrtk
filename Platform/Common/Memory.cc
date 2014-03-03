#include <PageFrameAllocator.hh>
#include <Parameters.hh>
#include <Memory.hh>
#include <Multiboot.hh>
#include <Debug.hh>
#include <Templates.hh>
#include <Modules.hh>
#include <Memory.hh>
#include <Hal.hh>

#include <X86/ThreadContext.hh>

#include <cstring>

//#define DEBUG

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

spinlock_softirq_t Memory::pagesLock;
spinlock_softirq_t Memory::memoryMapLock;

//#define DEBUG

void
Memory::init()
{
   printf("Listing Kernel sections:\n");
   printf(".text: %p-%p\n", &__start_text, &__end_text);
   printf(".data: %p-%p\n", &__start_data, &__end_data);
   printf(".rodata: %p-%p\n", &__start_rodata, &__end_rodata);
   printf(".bss: %p-%p\n", &__start_bss, &__end_bss);

   printf("Page size: %u, PhysicalPage structure size: %zu\n", PageSize, sizeof(PhysicalPage));

   spinlock_softirq_init(&pagesLock);
   spinlock_softirq_init(&memoryMapLock);

   usedPages.init();
   freePages.init();

   uintptr_t firstUsableMemory = max((uintptr_t )&__end_kernel, __end_modules);
   uintptr_t firstFreePage = roundTo<uintptr_t>(firstUsableMemory, PageSize) + PageSize * PageFrameCount;

   // init page clusters
   PhysicalPage* bootstrapPage = (PhysicalPage* )firstFreePage;
   bootstrapPage->init((uintptr_t )bootstrapPage - KernelVirtualBase);

   printf("Inserting bootstrap page to used pages cluster: %p\n", (void* )bootstrapPage->getAddress());
   usedPages.insert(bootstrapPage);
   
   unsigned int freeStructures = PageSize / sizeof (PhysicalPage) - 1;

   KASSERT(freeStructures > 0);
   printf("Free structures: %u\n", freeStructures);

   PhysicalPage* p = bootstrapPage + 1;
   for (unsigned int i = 0; i < memoryMapCount; i++)
   {
      printf("Adding memory range %p-%p (%zu)\n",
	     (void* )memoryMap[i].address,
	     (void* )(memoryMap[i].address + memoryMap[i].size),
	     memoryMap[i].size);

      for (uintptr_t addr = memoryMap[i].address;
	   addr < memoryMap[i].address + memoryMap[i].size;
	   addr += PageSize)
      {
	 if (freeStructures == 0)
	 {
#ifdef DEBUG
	    printf("Allocating new page structure\n");
#endif
	    //p = (PhysicalPage* )sbrk(PageSize)
	    uintptr_t ppage = getPage();
	    KASSERT(ppage != 0);
	    p = (PhysicalPage* )mapPage(ppage); // XXX make a function for this
	    KASSERT(p != 0);
	    freeStructures = PageSize / sizeof (PhysicalPage);
	 }

	 p->init(addr);
	 if ((addr >= KernelLoadAddress) && (addr < firstFreePage - KernelVirtualBase))
	 {
#ifdef DEBUG
	    printf("Found used page: %p\n", (void* )addr);
#endif
	    usedPages.insert(p);
	 }
	 else
	 {
#ifdef DEBUG
	    printf("Inserting free page: %p\n", (void* )addr);
#endif
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

   top = newStack;

   return true;
}
#endif

bool
Memory::mapPage(uintptr_t address, uintptr_t phys)
{
   bool success = Hal::mapPage(address, phys);

   if (success) {
      std::memset((void*)address, 0, PageSize);
   }

   return success;
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
   spinlock_softirq_enter(&memoryMapLock);

   mapEnd -= PageSize;
   uintptr_t virt = mapEnd;

   spinlock_softirq_exit(&memoryMapLock);

   if (virt <= (uintptr_t)&__end_kernel)
//   if (virt <= heapEnd)
   {
      printf("%p <= %p\n", (void*)virt, (void*)heapEnd);
      Debug::panic("Memory::mapPage: Kernel map namespace exhausted.");
   }

//   Debug::info("Mapping anonymous page: %p to %p\n", phys, mapEnd);
   bool success = mapPage(virt, phys);
   if (!success)
   {
      return 0;
   }

   return virt;
}

// anonymous memory mapping
//
uintptr_t Memory::mapAnonymousRegion(std::size_t size)
{
   std::size_t rsize = roundTo<uintptr_t>(size, PageSize);

   spinlock_softirq_enter(&memoryMapLock);

   mapEnd -= rsize;
   uintptr_t vaddr = mapEnd;

   spinlock_softirq_exit(&memoryMapLock);


   if (vaddr <= (uintptr_t)&__end_kernel)
//   if (vaddr <= heapEnd)
   {
      printf("%p <= %p\n", (void*)vaddr, (void*)heapEnd);
      Debug::panic("Memory::mapAnonymousRegion: Kernel map name space exhausted.");
   }

   for (std::size_t i = 0; i < rsize / PageSize; i++, vaddr += PageSize)
   {
      uintptr_t emptyPage = getPage();
      KASSERT(emptyPage != 0);
      bool success = mapPage(vaddr, emptyPage);
      if (!success)
      {
         Debug::panic("mapRegion unsuccesful.");
         // XXX unmap and return error
         //return 0;
      }
   }

   return mapEnd + rsize;
}

// anonymous mapping of a memory region
//
uintptr_t Memory::mapRegion(uintptr_t paddr, std::size_t size)
{
   std::size_t rsize = roundTo<uintptr_t>(size, PageSize);

   spinlock_softirq_enter(&memoryMapLock);

   mapEnd -= rsize;
   uintptr_t vaddr = mapEnd;

   spinlock_softirq_exit(&memoryMapLock);

   uintptr_t firstPage = roundDown(paddr, PageSize);
   uintptr_t offset = paddr - firstPage;

   if (vaddr <= (uintptr_t)&__end_kernel)
//   if (vaddr <= heapEnd)
   {
      printf("%p <= %p\n", (void*)vaddr, (void*)heapEnd);
      Debug::panic("Memory::mapRegion: Kernel map namespace exhausted.");
   }

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

// get a free physical page
//
uintptr_t
Memory::getPage()
{
   uintptr_t address = 0;

   spinlock_softirq_enter(&pagesLock);

   PhysicalPage* page = freePages.get();
   if (page != 0)
   {
      usedPages.insert(page);
      address = page->getAddress();
   }

   spinlock_softirq_exit(&pagesLock);

   return address;
}

// free a physical page
//
void
Memory::putPage(uintptr_t address)
{
   spinlock_softirq_enter(&pagesLock);

   PhysicalPage* page = usedPages.find(address);

   if (page == 0)
   {
      spinlock_softirq_exit(&pagesLock);
      Debug::panic("Trying to free an unallocated physical page: %p\n", (void* )address);
   }

   usedPages.remove(page);
   freePages.insert(page);

   spinlock_softirq_exit(&pagesLock);
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
