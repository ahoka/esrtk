#include <PageFrameAllocator.hh>
#include <Debug.hh>
#include <Parameters.hh>
#include <X86/Memory.hh>
#include <Templates.hh>

#include <cstdio>

//#define DEBUG

char* PageFrameAllocator::start;
char PageFrameAllocator::bitmap[PageFrameCount];

void*
PageFrameAllocator::getFreePage()
{
   for (unsigned int i = 0; i < sizeof bitmap; i++)
   {
#ifdef DEBUG
      printf("Checking for free page: %u: %hhx\n", i, bitmap[i]);
#endif

      if (bitmap[i] == 0)
      {
         bitmap[i] = 1;

         void* page = (void* )(start + PageSize * i);
#ifdef DEBUG
         printf("Found free page at %u: %p\n", i, page);
#endif
         KASSERT(((unsigned long )page & PageMask) == 0);

         return page;
      }
   }

   return 0;
}

void
PageFrameAllocator::putFreePage(void* page)
{
   KASSERT(((unsigned long )page & PageMask) == 0);

   int index = ((unsigned long )page - (unsigned long )start) / PageSize;

   bitmap[index] = 0;
}

void
PageFrameAllocator::init()
{
   for (unsigned int i = 0; i < sizeof bitmap; i++)
   {
      bitmap[i] = 0;
   }

   start = roundTo<char*>((char* )&__end_kernel, PageSize);

   printf("Page frame allocator pool from %p to %p (end of .bss is at %p)\n", start, start + PageFrameCount * PageSize, (void* )&__end_bss);
}
