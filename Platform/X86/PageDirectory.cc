#include <X86/CpuRegisters.hh>
#include <PageFrameAllocator.hh>
#include <X86/Parameters.hh>
#include <X86/Assembly.hh>
#include <Debug.hh>

#include <X86/PageDirectory.hh>

#include <cstring>
#include <cstdio>

//#define DEBUG

// static uint32_t
// phystov(uint32_t p)
// {
//    return p + KernelVirtualBase;
// }

static uint32_t*
phystov(uint32_t* p)
{
   return (uint32_t *)((uint32_t )p + KernelVirtualBase);
}

static uint32_t*
vtophys(uint32_t* p)
{
   return (uint32_t *)((uint32_t )p - KernelVirtualBase);
}

static void*
vtophys(void* p)
{
   return (void* )((uint32_t )p - KernelVirtualBase);
}

static uint32_t
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
   pageDirectory[1023] = (uint32_t )vtophys(pageDirectory) | 0x1;

   // map kernel memory
   //
   printf("Creating kernel memory mapping\n");
   for (uint32_t i = 0; i < KernelMemorySize / PageSize; i++)
   {
      bool rc = mapPage((KernelVirtualBase + i * PageSize),
                        (i * PageSize),
                        (uint32_t **)pageDirectory);
      KASSERT(rc);
   }

   // id map it too
   // TODO unmap after initPaging
   for (uint32_t i = 0; i < KernelMemorySize / PageSize; i++)
   {
      bool rc = mapPage((KernelLoadAddress + i * PageSize),
                        (KernelLoadAddress + i * PageSize),
                        (uint32_t **)pageDirectory);
      KASSERT(rc);
   }

   printf("Setting kernel page directory to %p\n", vtophys(pageDirectory));

   // call it via identity mapped address
   //
   void (*initp)(uint32_t) = (void (*)(uint32_t))vtophys((void*)&initPaging);
   initp(vtophys((uint32_t )pageDirectory));

//   mapPage(0x82002000u, 0x82000000u);

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

uint32_t
PageDirectory::addressToPdeIndex(uint32_t address)
{
   unsigned long index = (unsigned long )address;

   index >>= 22;

   return index;
}

uint32_t
PageDirectory::addressToPteIndex(uint32_t address)
{
   unsigned long index = (unsigned long )address;

   index >>= 12;
   index &= 0x3ff;

   return index;
}

uint32_t*
PageDirectory::addressToPde(uint32_t address)
{
//   uint32_t index = address / 0x400000;
   uint32_t index = addressToPdeIndex(address);

   printf("pde idx: %u, 0x%x + 0x%x\n", index, PageDirectoryBase, index * 4);

   return (uint32_t* )(PageDirectoryBase + index * 4);
}

uint32_t*
PageDirectory::addressToPte(uint32_t address)
{
//   uint32_t index = (address % 0x400000) / 0x1000;
   uint32_t pdeIndex = addressToPdeIndex(address);
   uint32_t pteIndex = addressToPteIndex(address);

   printf("pte idx: %u, 0x%x + 0x%x + 0x%x\n", pteIndex, PageTableBase, pdeIndex * 0x1000, pteIndex * 4);

   return (uint32_t* )(PageTableBase + pdeIndex * 0x1000 + pteIndex * 4);
}

bool
PageDirectory::mapPage(uint32_t vAddress, uint32_t pAddress, uint32_t** pageDirectory)
{
   uint32_t* pt = pageDirectory[addressToPdeIndex(vAddress)];

#ifdef DEBUG
   printf("Mapping %p to %p\n", (void* )vAddress, (void* )pAddress);
   printf("Page directory: %p, PDE at %p\n", pageDirectory, addressToPdeIndex(vAddress));
#endif

   if (((unsigned long)pt & 0x1) == 0)
   {
      // allocate new
      uint32_t* newPt = (uint32_t* )PageFrameAllocator::getFreePage();
      printf("allocating new page table: %p\n", newPt);

      memset(newPt, 0, PageSize);

      // mark present
      newPt = (uint32_t* )((uint32_t )newPt | 0x3);
      pt = vtophys(newPt);

      pageDirectory[addressToPdeIndex(vAddress)] = pt;

#ifdef DEBUG
      printf("pt is now at: v:%p p:%p\n", phystov(pageDirectory[addressToPageDirectory(vAddress)]),
             pageDirectory[addressToPageDirectory(vAddress)]);
#endif
   }

   pt = (uint32_t *)((uint32_t )pt & 0xfffff000);

   uint32_t* vpt = phystov(pt);
#ifdef DEBUG
   printf("Page table entry: (v:%p p:%p) %p\n", vpt, pt, (void*)vpt[addressToPteIndex(vAddress)]);
#endif
   if (vpt[addressToPteIndex(vAddress)] != 0)
   {
      // already mapped
      return false;
   }

   vpt[addressToPteIndex(vAddress)] = (unsigned long )pAddress | 0x3;

#ifdef DEBUG
   printf("New entry at %lu is: %p\n", addressToPte(vAddress), (void*)vpt[addressToPte(vAddress)]);
#endif

   return true;
}

bool
PageDirectory::unmapPage(uint32_t vAddress, uint32_t** pageDirectory)
{
   uint32_t* pt = pageDirectory[addressToPdeIndex(vAddress)];

#ifdef DEBUG
   printf("Unmapping %p\n", (void* )vAddress);
   printf("Page directory: %p\n", pageDirectory);
#endif

   if (((unsigned long)pt & 0x1) == 0)
   {
      // wasnt mapped
      //
      return false;
   }

   uint32_t* vpt = phystov(pt);

#ifdef DEBUG
   printf("Page table entry: (v:%p p:%p) %p\n", vpt, pt, (void*)vpt[addressToPdeIndex(vAddress)]);
#endif

   if (vpt[addressToPteIndex(vAddress)] == 0)
   {
      // wasnt mapped
      return false;
   }

   vpt[addressToPteIndex(vAddress)] = 0;

   return true;
}

#define PageValid (1 << 0)
#define PageWritable (1 << 1)

bool
PageDirectory::mapPage(uint32_t vAddress, uint32_t pAddress)
{
   uint32_t* pde = addressToPde(vAddress);

   if ((*pde & PageValid) == 0)
   {
      // need to allocate the pde
      //
      uint32_t newPde = (uint32_t )PageFrameAllocator::getFreePage();
      KASSERT(newPde != 0);
      newPde = vtophys(newPde) | (PageValid | PageWritable);
      *pde = newPde;

      // XXX flush tlb?
   }

   uint32_t* pte = addressToPte(vAddress);

   if (*pte & PageValid)
   {
//      printf("already mapped!\n");
      return false;
   }

   *pte = (pAddress & ~0xfffu) | (PageValid | PageWritable);

   return true;
}
