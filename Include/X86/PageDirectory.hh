#ifndef PAGEDIRECTORY_HH
#define PAGEDIRECTORY_HH

#include <stdint.h>

class PageDirectory
{
public:
   static void init();
   static bool mapPage(uint32_t vAddress, uint32_t pAddress, uint32_t** pageDirectory = (uint32_t** )PageDirAddress);
   static bool unmapPage(uint32_t vAddress, uint32_t** pageDirectory = (uint32_t** )PageDirAddress);

private:
   enum
   {
      PageDirAddress = 0xffc00000
   };

   static unsigned long addressToPageDirectory(uint32_t address);
   static unsigned long addressToPageEntry(uint32_t address);
};

#endif
