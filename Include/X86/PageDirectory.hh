#ifndef PAGEDIRECTORY_HH
#define PAGEDIRECTORY_HH

#include <cstdint>

class PageDirectory
{
public:
   static void init();
   static bool mapPage(uint32_t vAddress, uint32_t pAddress, int flags = 0);
   static bool unmapPage(uint32_t vAddress);

   static uintptr_t getPageTableEntry(uint32_t vAddress);
   static uintptr_t getPhysicalPage(uint32_t vAddress);

   enum
   {
      MapLazy =	(1 << 0)
   };

private:

   enum
   {
      Present =		(1 << 0),
      Writable =	(1 << 1),
      User =		(1 << 2),
      WriteThrough =	(1 << 3),
      DontCache =	(1 << 4),
      Accesses =	(1 << 5),
      Size =		(1 << 6),
      Global =		(1 << 7),
      //
      // Implementation dependent
      //
      Lazy =		(1 << 8)
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
