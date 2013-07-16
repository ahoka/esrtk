#include <X86/CpuRegisters.hh>
#include <PageFrameAllocator.hh>
#include <Parameters.hh>

class PageDirectory
{
   void init()
   {
      uint32_t* pageDirectory = (uint32_t* )PageFrameAllocator::getFreePage();
      KASSERT(pageDirectory != 0);

      memset(pageDirectory, 0, PageSize);
   }
};
