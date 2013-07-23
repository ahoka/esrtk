#include <X86/CpuRegisters.hh>
#include <PageFrameAllocator.hh>
#include <X86/Parameters.hh>
#include <X86/Assembly.hh>
#include <Debug.hh>

#include <X86/PageDirectory.hh>

#include <string.h>
#include <stdio.h>

uint32_t
phystov(uint32_t p)
{
   return p + KernelVirtualBase;
}

uint32_t*
phystov(uint32_t* p)
{
   return (uint32_t *)((uint32_t )p + KernelVirtualBase);
}

uint32_t*
vtophys(uint32_t* p)
{
   return (uint32_t *)((uint32_t )p - KernelVirtualBase);
}

uint32_t
vtophys(uint32_t p)
{
   return p - KernelVirtualBase;
}

void
PageDirectory::init()
{
   PageFrameAllocator::init();

   uint32_t* pageDirectory = (uint32_t* )PageFrameAllocator::getFreePage();
   printf("Initing pageDirectory at %p\n", pageDirectory);
   KASSERT(pageDirectory != 0);

   memset(pageDirectory, 0, PageSize);

   // The last 4MB will be mapped to the page directory itself
   //
   void* last = (void* )PageDirAddress;
   printf("Mapping pageDirectory to %p\n", last);
   pageDirectory[addressToPageDirectory(last)] = (uint32_t )VTOPHYS(pageDirectory);

   // map kernel memory
   //
   printf("Creating kernel memory mapping\n");
   for (int i = 0; i < KernelMemorySize / PageSize; i++)
   {
      bool rc = mapPage((void *)(KernelVirtualBase + i * PageSize),
                        (void *)(KernelLoadAddress + i * PageSize),
                        (uint32_t **)pageDirectory);
      KASSERT(rc);
   }

   // printf("mapping finished, disable pe\n");

   // // disable page extension
   // //
   // uint32_t cr4 = getCr4();
   // cr4 &= ~(1 << 4);
   // setCr4(cr4);

   // asm volatile("hlt");

   // printf("enable paging\n");
   // // make sure paging is enabled
   // //
   // uint32_t cr0 = getCr0();
   // cr0 |= (1 << 31);
   // setCr0(cr0);

   // printf("setting cr3\n");
   // setCr3(VTOPHYS((uint32_t )pageDirectory));

   printf("enable paging: %p\n", pageDirectory);

   initPaging((uint32_t )pageDirectory);

   printf("lets hope this ever works\n");
}

unsigned long
PageDirectory::addressToPageDirectory(void* address)
{
   unsigned long index = (unsigned long )address;

   index >>= 22;

   return index;
}

unsigned long
PageDirectory::addressToPageEntry(void* address)
{
   unsigned long index = (unsigned long )address;

   index >>= 12;
   index &= 0x7ff;

   return index;
}

bool
PageDirectory::mapPage(void* vAddress, void* pAddress, uint32_t** pageDirectory)
{
   uint32_t* pt = pageDirectory[addressToPageDirectory(vAddress)];

   if (((unsigned long)pt & 0x1) == 0)
   {
      // allocate new
      uint32_t* newPt = (uint32_t* )PageFrameAllocator::getFreePage();
      printf("allocating new page table: %p\n", newPt);

      memset(PHYSTOV(newPt), 0, PageSize);

      // uint32_t** p = &pageDirectory[addressToPageDirectory(vAddress)];
      // uint32_t** pv = PHYSTOV(p);
      // printf("pv %p\n", pv);

      // mark present
      newPt = (uint32_t* )((uint32_t )newPt | 0x1);
      pageDirectory[addressToPageDirectory(vAddress)] = newPt;
      pt = newPt;

#ifdef DEBUG
      printf("pt is now at: v:%p p:%p\n", PHYSTOV(pageDirectory[addressToPageDirectory(vAddress)]),
             pageDirectory[addressToPageDirectory(vAddress)]);
#endif

#ifdef VERBOSE_DEBUG
      bool wasEmpty = false;
      bool wasDot = false;
      for (int i = 0; i < 4096; i += 4)
      {
         if (wasEmpty && pageDirectory[i] == 0 && pageDirectory[i+1] == 0 && pageDirectory[i+2] == 0 && pageDirectory[i+3] == 0)
         {
            if (!wasDot)
            {
               printf("...\n");
            }
            wasDot = true;
         }
         else
         {
            printf("%p %p %p %p\n", pageDirectory[i], pageDirectory[i+1], pageDirectory[i+2], pageDirectory[i+3]);
            wasDot = false;
         }

         if (pageDirectory[i] == 0 && pageDirectory[i+1] == 0 && pageDirectory[i+2] == 0 && pageDirectory[i+3] == 0)
         {
            wasEmpty = true;
         }
         else
         {
            wasEmpty = false;
         }
      }
#endif
   }

   pt = (uint32_t *)((uint32_t )pt & ~1);

   uint32_t* vpt = phystov(pt);
#ifdef DEBUG
   printf("Page table entry: (v:%p p:%p) %p\n", vpt, pt, (void*)vpt[addressToPageEntry(vAddress)]);
#endif
   if (vpt[addressToPageEntry(vAddress)] != 0)
   {
      // already mapped
      return false;
   }

   vpt[addressToPageEntry(vAddress)] = (unsigned long )pAddress | 0x3;

#ifdef DEBUG
   printf("New entry at %lu is: %p\n", addressToPageEntry(vAddress), (void*)vpt[addressToPageEntry(vAddress)]);
#endif

   return true;
}
