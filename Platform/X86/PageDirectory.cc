#include <X86/CpuRegisters.hh>
#include <PageFrameAllocator.hh>
#include <X86/Parameters.hh>
#include <X86/Assembly.hh>
#include <Debug.hh>

#include <X86/PageDirectory.hh>

#include <string.h>
#include <stdio.h>

void
PageDirectory::init()
{
   uint32_t* pageDirectory = (uint32_t* )PageFrameAllocator::getFreePage();
   KASSERT(pageDirectory != 0);

   memset(pageDirectory, 0, PageSize);

   // The last 4MB will be mapped to the page directory itself
   //
   void* last = (void* )PageDirAddress;
   pageDirectory[addressToPageDirectory(last)] = (uint32_t )VTOPHYS(pageDirectory);

   // map kernel memory
   //
   for (int i = 0; i < KernelMemorySize / PageSize; i++)
   {
      bool rc = mapPage((void *)(KernelVirtualBase + i * PageSize), (void *)(KernelLoadAddress + i * PageSize));
      KASSERT(rc);
   }

   // disable page extension
   //
   uint32_t cr4 = getCr4();
   cr4 &= ~(1 << 4);
   setCr4(cr4);

   // make sure paging is enabled
   //
   uint32_t cr0 = getCr0();
   cr0 |= (1 << 31);
   setCr0(cr0);

   setCr3(VTOPHYS((uint32_t )pageDirectory));

   printf("lets hope this ever works\n");
}

unsigned int
PageDirectory::addressToPageDirectory(void* address)
{
   long index = long(address);

   index >>= 22;

   return index;
}

unsigned int
PageDirectory::addressToPageEntry(void* address)
{
   long index = long(address);

   index >>= 12;
   index &= 0x7ff;

   return index;
}

bool
PageDirectory::mapPage(void* vAddress, void* pAddress)
{
   uint32_t** pageDirectory = (uint32_t **)PageDirAddress;
   uint32_t* pd = PHYSTOV(pageDirectory[addressToPageDirectory(vAddress)]);

   if (!((unsigned long)pd & 0x1))
   {
      // allocate new
      uint32_t* newPd = (uint32_t* )PageFrameAllocator::getFreePage();

      uint32_t** p = &pageDirectory[addressToPageDirectory(vAddress)];
      uint32_t** pv = PHYSTOV(p);

      *p = newPd;
   }

   if (pd[addressToPageEntry(vAddress)] != 0)
   {
      return false;
   }

   pd[addressToPageEntry(vAddress)] = (unsigned long )pAddress | 0x3;

   return true;
}
