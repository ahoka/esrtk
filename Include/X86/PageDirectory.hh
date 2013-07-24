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
      PageDirectoryBase = 0xffc00000
   };

   // for initial page directory
   //
   static bool mapPage(uint32_t vAddress, uint32_t pAddress, uint32_t** pageDirectory);
   static bool unmapPage(uint32_t vAddress, uint32_t** pageDirectory);

   static int addressToPdeIndex(uint32_t address);
   static int addressToPteIndex(uint32_t address);
   static uint32_t* addressToPde(uint32_t address);
   static uint32_t* addressToPte(uint32_t address);
};

#endif
