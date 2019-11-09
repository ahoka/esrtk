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
         Uncacheable = (1 << 1),
         User = (1 << 2)
      };
   };

   void init();
   bool mapPage(uint32_t vAddress, uint32_t pAddress, int flags = Flags::None);
   bool unmapPage(uint32_t vAddress);
  bool isMapped(uint32_t vAddress);

   uintptr_t getPageTableEntry(uint32_t vAddress);
   uintptr_t getPhysicalPage(uint32_t vAddress);
   uintptr_t getKernelPageDirectory();

   uintptr_t createPageDirectory();
};

#endif
