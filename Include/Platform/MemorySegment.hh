#ifndef MEMORYSEGMENT_HH
#define MEMORYSEGMENT_HH

#include <cstdint>
#include <cstddef>

struct MemorySegment
{
   uintptr_t address;
   std::size_t size;
   uint8_t* bitmap;

   MemorySegment() :
      address(0),
      size(0),
      bitmap(0)
   {
   }
};

#endif
