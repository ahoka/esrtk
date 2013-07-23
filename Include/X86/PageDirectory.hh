#ifndef PAGEDIRECTORY_HH
#define PAGEDIRECTORY_HH

#include <stdint.h>

class PageDirectory
{
public:
   void init();
   bool mapPage(void* vAddress, void* pAddress);

private:
   enum
   {
      PageDirAddress = (~0ul - 4 * 1024 * 1024)
   };

   unsigned int addressToPageDirectory(void* address);
   unsigned int addressToPageEntry(void* address);
};

#endif
