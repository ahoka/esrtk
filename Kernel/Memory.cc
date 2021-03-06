#include <PageFrameAllocator.hh>
#include <Parameters.hh>
#include <Memory.hh>
#include <Multiboot.hh>
#include <Debug.hh>
#include <Templates.hh>
#include <Modules.hh>
#include <Memory.hh>

#include <X86/ThreadContext.hh>
#include <X86/PageDirectory.hh>

#include <cstring>

#undef DEBUG

#ifdef DEBUG
# define D(x) x
#else
# define D(x)
#endif

namespace
{
   uintptr_t heapEnd = KernelHeapStart;
   uintptr_t stackEnd = KernelStackStart;
   uintptr_t mapEnd = MapStart;

   MemorySegment memoryMap[MemoryMapMax];
   unsigned int memoryMapCount = 0;

   PageCluster usedPages;
   PageCluster freePages;

   spinlock_softirq_t pagesLock;
   spinlock_softirq_t memoryMapLock;
};

void
Memory::addMemoryMapEntry(uintptr_t start, size_t length)
{
   KASSERT(memoryMapCount < MemoryMapMax);

   memoryMap[memoryMapCount].address = start;
   memoryMap[memoryMapCount].size = length;
   memoryMapCount++;
}

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
	    D(printf("Allocating new page structure\n"));
	    //p = (PhysicalPage* )sbrk(PageSize)
	    uintptr_t ppage = getPage();
	    KASSERT(ppage != 0);
	    p = (PhysicalPage* )mapAnonymousPage(ppage); // XXX make a function for this
	    KASSERT(p != 0);
	    freeStructures = PageSize / sizeof (PhysicalPage);
	 }

	 p->init(addr);
	 if ((addr >= KernelLoadAddress) && (addr < firstFreePage - KernelVirtualBase))
	 {
	    D(printf("Found used page by kernel: %p\n", (void* )addr));
	    usedPages.insert(p);
	 }
         else if (__end_modules != 0u && addr >= __start_modules && addr < __end_modules)
         {
	    D(printf("Found used page by modules: %p\n", (void* )addr));
	    usedPages.insert(p);
         }
	 else
	 {
	    D(printf("Inserting free page: %p\n", (void* )addr));
	    freePages.insert(p);
	 }

	 p++;
	 freeStructures--;
      }
   }

   uintptr_t start = stackEnd;
   printf("Creating initial kernel stack: %p-%p (%u)\n",
          (void* )start, (void* )(start - KernelStackSize), KernelStackSize);

   for (uintptr_t stackAddress = (start - KernelStackSize);
	stackAddress < start;
	stackAddress += PageSize)
   {
      uintptr_t stackPage = getPage();
      KASSERT(stackPage != 0);

      bool success = mapPage(stackAddress, stackPage);
      KASSERT(success);
   }

   // add one unmapped page as guard
   stackEnd = start - KernelStackSize - PageSize;

   printf("Creating code area: %p-%p\n", (void*)CodeStart, (void*)(CodeStart + CodeSize));
   for (uintptr_t p = CodeStart; p < CodeStart + CodeSize; p += PageSize)
   {
      uintptr_t page = Memory::getPage();
      KASSERT(page != 0);
      bool rc = mapPage(p, page, Memory::MapUser);
      KASSERT(rc);
   }

   // XXX
   // memcpy((void*)CodeStart, "\xb8\xfa\x00\x00\x10\x00\xe0\xff", 8);
//   memset((void*)CodeStart, 90, 256);
   memcpy((void*)CodeStart, "\x31\xc0\xcd\x80", 4);

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

   printf("Creating new kernel stack: %p-%p (%u)\n", (void* )start,
          (void* )(start - KernelStackSize), KernelStackSize);

   for (uintptr_t stackAddress = (start - KernelStackSize);
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

   uintptr_t bottom = mapAnonymousPage(stackPage);
   KASSERT(bottom != 0);

   top = bottom + PageSize;

   printf("Creating new kernel stack: %p-%p (%u)\n", (void* )top, (void* )(bottom), PageSize);

   return true;
}
#endif

// anonymous mapping of a physical page
//
uintptr_t
Memory::mapAnonymousPage(uintptr_t phys, int flags)
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
   bool success = mapPage(virt, phys, flags);
   if (!success)
   {
      return 0;
   }

   return virt;
}

// anonymous memory mapping
//
uintptr_t Memory::mapAnonymousRegion(size_t size, int flags)
{
   KASSERT(size != 0);

   size_t rsize = roundTo<uintptr_t>(size, PageSize);

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

   for (size_t page = 0; page < rsize; page += PageSize)
   {
      uintptr_t emptyPage = getPage();
      KASSERT(emptyPage != 0);
      bool success = mapPage(vaddr + page, emptyPage, flags);
      if (!success)
      {
         Debug::panic("mapRegion unsuccesful.");
         // XXX unmap and return error
         //return 0;
      }
   }

   return vaddr;
}

// anonymous mapping of a memory region
//
uintptr_t Memory::mapRegion(uintptr_t paddr, size_t size, int flags)
{
   KASSERT(size != 0);

   uintptr_t firstPage = roundDown(paddr, PageSize);
   uintptr_t lastPage = roundTo(paddr + size, PageSize);
   size_t rsize = lastPage - firstPage;

   D(printf("Memory::mapRegion: %p-%p\n", (void*)paddr, (void*)(paddr + size)));
   D(printf("Memory::mapRegion: mapping: %p-%p (%zu)\n", (void*)firstPage, (void*)lastPage, rsize));

   spinlock_softirq_enter(&memoryMapLock);

   mapEnd -= rsize;
   uintptr_t vaddr = mapEnd;

   spinlock_softirq_exit(&memoryMapLock);

   uintptr_t offset = paddr - firstPage;

   if (vaddr <= (uintptr_t)&__end_kernel)
//   if (vaddr <= heapEnd)
   {
      printf("%p <= %p\n", (void*)vaddr, (void*)heapEnd);
      Debug::panic("Memory::mapRegion: Kernel map namespace exhausted.");
   }

   for (uintptr_t page = firstPage; page != lastPage; page += PageSize, vaddr += PageSize)
   {
      bool success = mapPage(vaddr, page, flags);
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
   KASSERT(size != 0);

   uintptr_t firstPage = roundDown(paddr, PageSize);
   uintptr_t lastPage = roundTo(paddr + size, PageSize);

   for (uintptr_t page = firstPage; page != lastPage; page += PageSize)
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
   D(printf("Memory::getPage\n"));

   uintptr_t address = 0;

   spinlock_softirq_enter(&pagesLock);

   if (freePages.count() == 0)
   {
      Debug::panic("Out of memory, free page cluster is empty");
   }

   PhysicalPage* page = freePages.get();
   if (page != 0)
   {
      usedPages.insert(page);
      address = page->getAddress();
   }

   spinlock_softirq_exit(&pagesLock);

   D(printf("Memory::getPage done: %p\n", (void*)address));

   return address;
}

// free a physical page
//
void
Memory::putPage(uintptr_t address)
{
   D(printf("Memory::putPage: %p\n", (void*)address));

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
   KASSERT(size != 0);

   uintptr_t mapFirst = roundDown(uintptr_t(source), PageSize);
   uintptr_t mapLast = roundTo(uintptr_t(source) + size, PageSize);

   uintptr_t mapped = mapRegion(mapFirst, mapLast - mapFirst);

   std::memcpy(destination, reinterpret_cast<void*>(mapped + (uintptr_t(source) - mapFirst)), size);

   unmapRegion(mapped, mapLast - mapFirst);

   return destination;
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

void
Memory::info()
{
   printf("Memory information:\n");

   printf("Free pages: %lu\n", freePages.count());
   printf("Used pages: %lu\n", usedPages.count());
}
