#ifndef SEGMENT_HH
#define SEGMENT_HH

#include <Debug.hh>

#include <cstddef>
#include <cstdint>

class SegmentList;

class Segment
{
public:
   Segment() :
      sizeM(0),
      prevM(this),
      nextM(this),
      allocated(false),
      checksum(0)
   {
      updateChecksum();
   }

   void* operator new(std::size_t, void* storage)
   {
      return storage;
   }

   void* operator new(std::size_t size)
   {
      return ::operator new(size);
   }

   void setSize(uintptr_t newSize)
   {
      sizeM = newSize;
   }

   std::size_t getSize() const
   {
      return sizeM;
   }

   uintptr_t getAddress() const
   {
      return (uintptr_t(this) + sizeof(*this));
   }

   bool isAllocated() const
   {
      return allocated;
   }

   void markAllocated()
   {
      allocated = true;
   }

   void markUnallocated()
   {
      allocated = false;
   }

   void updateChecksum()
   {
      checksum = 0;

      const uint8_t* p = (const uint8_t*)this;
      uint8_t sum = 0;
      for (std::size_t i = 0; i < sizeof(*this); i++, p++)
      {
	 sum ^= *p;
      }

      checksum = sum;
   }

   bool verifyChecksum() const
   {
      const uint8_t* p = (const uint8_t*)this;
      uint8_t sum = 0;
      for (std::size_t i = 0; i < sizeof(*this); i++, p++)
      {
	 sum ^= *p;
      }

      return sum == 0;
   }

   void dump() const
   {
      Debug::info("segment:          %p\n"
		  "address:          %p\n"
		  "size:             %zu\n"
		  "allocated:        %d\n"
		  "checksum:         0x%x\n"
		  "prev:	     %p\n"
		  "next:	     %p\n",
		  this,
		  (void*)getAddress(),
		  sizeM,
		  allocated,
		  checksum,
		  prevM,
		  nextM);
   }

private:
   std::size_t sizeM;

   Segment* prevM;
   Segment* nextM;

   bool allocated;
   uint8_t checksum;
   friend class SegmentList;
} __attribute__((aligned(16)));

#endif
