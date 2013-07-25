#ifndef PAGEDIRECTORY_HH
#define PAGEDIRECTORY_HH

#include <cstdint>

class PageDirectory
{
public:
   static void init();
   static bool mapPage(uint32_t vAddress, uint32_t pAddress);
   static bool unmapPage(uint32_t vAddress);

private:
   enum
   {
      PageTableBase = 0xffc00000u,
      PageDirectoryBase = 0xfffff000u
   };

   // for initial page directory
   //
   static bool mapPage(uint32_t vAddress, uint32_t pAddress, uint32_t** pageDirectory);
   static bool unmapPage(uint32_t vAddress, uint32_t** pageDirectory);

   static uint32_t addressToPdeIndex(uint32_t address);
   static uint32_t addressToPteIndex(uint32_t address);
   static uint32_t* addressToPde(uint32_t address);
   static uint32_t* addressToPte(uint32_t address);
};

#endif
