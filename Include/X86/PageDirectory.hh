#ifndef PAGEDIRECTORY_HH
#define PAGEDIRECTORY_HH

#include <stdint.h>

class PageDirectory
{
public:
   static void init();
   static bool mapPage(void* vAddress, void* pAddress, uint32_t** pageDirectory = (uint32_t** )PageDirAddress);

private:
   enum
   {
      PageDirAddress = (~0ul - 4 * 1024 * 1024)
   };

   static unsigned long addressToPageDirectory(void* address);
   static unsigned long addressToPageEntry(void* address);
};

#endif
