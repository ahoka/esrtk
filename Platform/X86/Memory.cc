#include <PageFrameAllocator.hh>
#include <Parameters.hh>
#include <Memory.hh>
#include <Multiboot.hh>
#include <Debug.hh>
#include <Templates.hh>
#include <Modules.hh>

#include <X86/Memory.hh>
#include <X86/Idt.hh>
#include <X86/PageDirectory.hh>

#include <cstring>

extern Multiboot* mbd;
extern uintptr_t initial_stack;

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

   // map memory for the kernel stack
   //
   printf("Mapping memory for the kernel stack: %p-%p (%u)\n", (void* )(StackStart - StackSize), (void* )StackStart, StackSize);
   for (uintptr_t stackAddress = StackStart - StackSize;
	stackAddress < StackStart;
	stackAddress += PageSize)
   {
      uintptr_t stackPage = getPage();
      KASSERT(stackPage != 0);

      bool success = mapPage(stackAddress, stackPage);
      KASSERT(success);
   }

//    printf("Freeing initial kernel stack: %p-%p\n", (void* )(initial_stack - InitialStackSize), (void* )initial_stack);
//    for (uintptr_t oldStack = initial_stack - InitialStackSize;
// 	oldStack < initial_stack;
// 	oldStack += PageSize)
//    {
//      unmap(oldStack);
//    }
}

bool Memory::mapPage(uintptr_t address, uintptr_t phys)
{
   return PageDirectory::mapPage(address, phys);
}

// anonymous mapping of a physical page
//
uintptr_t Memory::mapPage(uintptr_t phys)
{
   mapEnd -= PageSize;

   if (mapEnd <= heapEnd)
   {
      Debug::panic("Kernel map namespace exhausted.");
   }

   bool success = PageDirectory::mapPage(mapEnd, phys);
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
      bool success = PageDirectory::mapPage(vaddr, page);
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
      bool success = PageDirectory::unmapPage(page);
      if (!success)
      {
	 Debug::panic("unmapRegion unsuccesful.");
      }
   }

   return true;
}

// anonymous mapping of a physical page
//
bool
Memory::unmapPage(uintptr_t page)
{
   bool success = PageDirectory::unmapPage(page);
   // TODO we need to know where to mark the vaddr free!

   return success;
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

   KASSERT(foundUsableMemory);
}
