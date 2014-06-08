#ifndef PAGEDIRECTORY_HH
#define PAGEDIRECTORY_HH

#include <cstdint>

namespace PageDirectory
{
   namespace Flags
   {
      enum
      {
         None = 0,
         Lazy = (1 << 0),
         Uncachable = (1 << 1)
      };
   };

   void init();
   bool mapPage(uint32_t vAddress, uint32_t pAddress, int flags = Flags::None);
   bool unmapPage(uint32_t vAddress);

   uintptr_t getPageTableEntry(uint32_t vAddress);
   uintptr_t getPhysicalPage(uint32_t vAddress);
};

#endif
