#include <PageFrameAllocator.hh>
#include <Debug.hh>
#include <Parameters.hh>

extern char* __ebss;

char* PageFrameAllocator::start;
char PageFrameAllocator::bitmap[1024];

void *PageFrameAllocator::getFreePage()
{
   for (unsigned int i = 0; i < sizeof bitmap; i++)
   {
      if (bitmap[i] == 0)
      {
         bitmap[i] = 1;

         void* page = (void* )(start + PageSize * i);
         KASSERT(((unsigned long )page & PageMask) == 0);

         return page;
      }
   }

   return 0;
}

void PageFrameAllocator::putFreePage(void* page)
{
   KASSERT(((unsigned long )page & PageMask) == 0);

   int index = ((unsigned long )page - (unsigned long )start) / PageSize;

   bitmap[index] = 0;
}

void PageFrameAllocator::init()
{
   for (unsigned int i = 0; i < sizeof bitmap; i++)
   {
      bitmap[i] = 0;
   }

   start = (char* )(((unsigned long )__ebss + PageMask) & ~PageMask);
}
