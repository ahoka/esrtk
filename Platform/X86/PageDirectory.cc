#include <X86/CpuRegisters.hh>
#include <PageFrameAllocator.hh>
#include <Parameters.hh>
#include <Debug.hh>

#include <stdint.h>
#include <string.h>

class PageDirectory
{
   enum
   {
      PageDirAddress = (~0ul - 4 * 1024 * 1024)
   };

   void init()
   {
      uint32_t* pageDirectory = (uint32_t* )PageFrameAllocator::getFreePage();
      KASSERT(pageDirectory != 0);

      memset(pageDirectory, 0, PageSize);

      // The last 4MB will be mapped to the page directory itself
      //
      void* last = (void* )PageDirAddress;
      pageDirectory[addressToPageDirectory(last)] = (uint32_t )VTOPHYS(pageDirectory);
   }

   unsigned int addressToPageDirectory(void* address)
   {
      long index = long(address);

      index >>= 22;

      return index;
   }

   unsigned int addressToPageEntry(void* address)
   {
      long index = long(address);

      index >>= 12;
      index &= 0x7ff;

      return index;
   }

   bool mapPage(void* vAddress, void* pAddress)
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
};
