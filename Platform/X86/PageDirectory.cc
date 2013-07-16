#include <X86/CpuRegisters.hh>
#include <PageFrameAllocator.hh>
#include <Parameters.hh>
#include <Debug.hh>

#include <stdint.h>
#include <string.h>

class PageDirectory
{
   void init()
   {
      uint32_t* pageDirectory = (uint32_t* )PageFrameAllocator::getFreePage();
      KASSERT(pageDirectory != 0);

      memset(pageDirectory, 0, PageSize);
   }
};
