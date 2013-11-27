#include <Debug.hh>
#include <Templates.hh>
#include <Memory.hh>
#include <PageFrameAllocator.hh>
#include <Modules.hh>
#include <Memory.hh>

#include <X86/PageDirectory.hh>
#include <X86/Parameters.hh>
#include <X86/Assembly.hh>
#include <X86/CpuRegisters.hh>

#include <cstring>
#include <cstdio>

//#define DEBUG

#define PageValid (1 << 0)
#define PageWritable (1 << 1)

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

   std::memset(pageDirectory, 0, PageSize);

   // The last 4MB will be mapped to the page directory itself
   //
   pageDirectory[1023] = (uint32_t )vtophys(pageDirectory) | Present;

   // map kernel memory
   //
   uintptr_t first = KernelLoadAddress;

   // kernel + initial page directory entries + physmap bootstrap page

   uintptr_t highestKernelAddress = max((uintptr_t )&__end_kernel, __end_modules);
   uintptr_t last = (roundTo<uintptr_t>(highestKernelAddress, PageSize)
		     + PageSize * PageFrameCount + PageSize) - KernelVirtualBase;

   printf("Creating kernel memory mapping: %p-%p\n", (void* )first, (void* )last);
   for (uintptr_t p = first; p < last; p += PageSize)
   {
      bool rc = mapPage((KernelVirtualBase + p), p, (uint32_t **)pageDirectory);
      KASSERT(rc);
   }

   // id map it too
   printf("Creating kernel memory mapping (identity): %p-%p\n", (void* )first, (void* )last);
   for (uintptr_t p = first; p < last; p += PageSize)
   {
      bool rc = mapPage(p, p, (uint32_t **)pageDirectory);
      KASSERT(rc);
   }

   printf("Setting kernel page directory to %p\n", vtophys(pageDirectory));

   // call it via identity mapped address
   //
   void (*initp)(uint32_t) = (void (*)(uint32_t))vtophys((void*)&initPaging);
   initp(vtophys((uint32_t )pageDirectory));

   // unmap identity mapped region
   //
   printf("Unmapping identity mapped kernel memory: %p-%p\n", (void* )first, (void* )last);
   for (uintptr_t p = first; p < last; p += PageSize)
   {
      bool rc = unmapPage(p);
      KASSERT(rc);
   }

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
   uint32_t index = addressToPdeIndex(address);

   return (uint32_t* )(PageDirectoryBase + index * 4);
}

uint32_t*
PageDirectory::addressToPte(uint32_t address)
{
   uint32_t pdeIndex = addressToPdeIndex(address);
   uint32_t pteIndex = addressToPteIndex(address);

   return (uint32_t* )(PageTableBase + pdeIndex * 0x1000 + pteIndex * 4);
}

bool
PageDirectory::mapPage(uint32_t vAddress, uint32_t pAddress, uint32_t** pageDirectory)
{
   KASSERT((vAddress & PageMask) == 0);
   KASSERT((pAddress & PageMask) == 0);
   uint32_t* pt = pageDirectory[addressToPdeIndex(vAddress)];

#ifdef DEBUG
   printf("Mapping %p to %p (initial)\n", (void* )vAddress, (void* )pAddress);
#endif

   if (((unsigned long)pt & PageValid) == 0)
   {
      // allocate new
      uint32_t* newPt = (uint32_t* )PageFrameAllocator::getFreePage();
#ifdef DEBUG
      printf("allocating new page table: %p\n", newPt);
#endif

      std::memset(newPt, 0, PageSize);

      // mark present
      newPt = (uint32_t* )((uint32_t )newPt | Present | Writable);
      pt = vtophys(newPt);

      pageDirectory[addressToPdeIndex(vAddress)] = pt;
   }

   pt = (uint32_t *)((uint32_t )pt & 0xfffff000);

   uint32_t* vpt = phystov(pt);
   if (vpt[addressToPteIndex(vAddress)] != 0)
   {
      // already mapped
      Debug::panic("Trying to map %p to the already used address %p\n", (void* )pAddress, (void* )vAddress);
//      return false;
   }

   vpt[addressToPteIndex(vAddress)] = (unsigned long )pAddress | Present | Writable;

   invlpg(vAddress);

   return true;
}

bool
PageDirectory::unmapPage(uint32_t vAddress, uint32_t** pageDirectory)
{
   KASSERT((vAddress & PageMask) == 0);

   uint32_t* pt = pageDirectory[addressToPdeIndex(vAddress)];

#ifdef DEBUG
   printf("Unmapping %p\n", (void* )vAddress);
   printf("Page directory: %p\n", pageDirectory);
#endif

   if (((unsigned long)pt & Present) == 0)
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

   invlpg(vAddress);

   return true;
}

bool
PageDirectory::mapPage(uint32_t vAddress, uint32_t pAddress)
{
   KASSERT((vAddress & PageMask) == 0);
   KASSERT((pAddress & PageMask) == 0);

#ifdef DEBUG
   printf("Mapping %p to %p\n", (void* )vAddress, (void* )pAddress);
#endif

   uint32_t* pde = addressToPde(vAddress);

#ifdef DEBUG
   printf("pde is at %p, content 0x%x\n", pde, *pde);
#endif

   if ((*pde & PageValid) == 0)
   {
      // need to allocate the pde
      //
//      uint32_t newPde = (uint32_t )PageFrameAllocator::getFreePage();
      uint32_t newPde = Memory::getPage();
#ifdef DEBUG
      printf("got new page for pde: %p\n", (void *)newPde);
#endif
      KASSERT(newPde != 0);
      newPde |= (PageValid | PageWritable);
#ifdef DEBUG
      printf("writing to pde: %p\n", (void *)newPde);
#endif
      *pde = newPde;

      uintptr_t newpte = (uintptr_t )addressToPte(vAddress) & ~PageMask;
      invlpg(newpte);

      std::memset((void *)newpte, 0, PageSize);
   }

   uint32_t* pte = addressToPte(vAddress);
#ifdef DEBUG
   printf("pte is at %p, content 0x%x\n", pte, *pte);
#endif

   if (*pte & PageValid)
   {
      Debug::panic("Trying to map %p to the already used address %p\n", (void* )pAddress, (void* )vAddress);
//      return false;
   }

   *pte = (pAddress & ~0xfffu) | (PageValid | PageWritable);
   invlpg(vAddress);

#ifdef DEBUG
   printf("Results: pde %p, pte %p\n", (void* )*pde, (void* )*pte);
#endif

   return true;
}

bool
PageDirectory::unmapPage(uint32_t vAddress)
{
   KASSERT((vAddress & PageMask) == 0);
#ifdef DEBUG
   printf("Unmapping page: %p\n", (void* )vAddress);
#endif


   uint32_t* pde = addressToPde(vAddress);

   if ((*pde & PageValid) == 0)
   {
      Debug::panic("Trying to unmap a not mapped page: %p (invalid PDE)\n", (void* )vAddress);
//      return false;
   }

   uint32_t* pte = addressToPte(vAddress);

   if ((*pte & PageValid) == 0)
   {
      Debug::panic("Trying to unmap a not mapped page: %p (invalid PTE: 0x%x)\n", (void* )vAddress, *pte);
//      return false;
   }

   // TODO free page directory if empty
   *pte = PageWritable;

   invlpg(vAddress);

   return true;
}
