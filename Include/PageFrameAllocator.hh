#ifndef PAGEFRAMEALLOCATOR_HH
#define PAGEFRAMEALLOCATOR_HH

class PageFrameAllocator
{
public:
   static void *getFreePage();
   static void putFreePage(void* page);
   static void init();

private:
   PageFrameAllocator();

   static char* start;
   static char bitmap[1024];
};

#endif

