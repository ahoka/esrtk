#ifndef PAGEDIRECTORY_HH
#define PAGEDIRECTORY_HH

#include <stdint.h>

class PageDirectory
{
public:
   static void init();
   static bool mapPage(void* vAddress, void* pAddress, uint32_t** pageDirectory = (uint32_t** )PageDirAddress);
   static bool unmapPage(void* vAddress, uint32_t** pageDirectory = (uint32_t** )PageDirAddress);

private:
   enum
   {
//      PageDirAddress = (~0ul - 4 * 1024 * 1024)
      PageDirAddress = 0xffc00000
   };

   static unsigned long addressToPageDirectory(void* address);
   static unsigned long addressToPageEntry(void* address);
};

#endif
