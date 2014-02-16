#ifndef SEGMENT_HH
#define SEGMENT_HH

class SegmentList;

class Segment
{
public:
   Segment() :
      size(0),
      prevSegment(this),
      nextSegment(this),
      allocated(false),
      checksum(0)
   {
      Debug::verbose("Segment(%p)\n", this);
//      updateChecksum();
      dump();
   }

   void* operator new (std::size_t, void* storage)
   {
      return storage;
   }

   void setSize(uintptr_t newSize)
   {
      KASSERT(verifyChecksum());

      size = newSize;

      updateChecksum();
   }

   std::size_t getSize()
   {
      KASSERT(verifyChecksum());

      return size;
   }

   uintptr_t getAddress()
   {
      KASSERT(verifyChecksum());

      return (uintptr_t(this) + sizeof(*this));
   }

   bool isAllocated()
   {
      KASSERT(verifyChecksum());

      return allocated;
   }
      
   void markAllocated()
   {
      KASSERT(verifyChecksum());

      allocated = true;

      updateChecksum();
   }

   void markUnallocated()
   {
      KASSERT(verifyChecksum());

      allocated = false;

      updateChecksum();
   }

   void updateChecksum()
   {
      checksum = 0;

      const uint8_t* p = (const uint8_t*)this;
      uint8_t sum = 0;
      for (std::size_t i = 0; i < sizeof(*this); i++, p++)
      {
	 sum ^= *p;
//	 Debug::verbose("0x%x -> 0x%x\n", *p, sum);
      }

//      Debug::verbose("Checksum updated: 0x%x\n", sum);
      dump();

      checksum = sum;

      /// XXX debug
      verifyChecksum();
   }

   bool verifyChecksum() const
   {
      return true;
      const uint8_t* p = (const uint8_t*)this;
      uint8_t sum = 0;
      for (std::size_t i = 0; i < sizeof(*this); i++, p++)
      {
	 sum ^= *p;
//	 Debug::verbose("0x%x -> 0x%x\n", *p, sum);
      }

      if (sum != 0)
      {
	 dump();
	 Debug::panic("Allocator checksum error, possible memory corruption: 0x%x\n", sum);
      }
      else
      {
	 Debug::verbose("Checksum OK\n");
	 return true;
      }
   }

   void dump() const
   {
      Debug::info("segment:          %p\n"
//		  "address:          %p\n"
		  "size:             %zu\n"
		  "allocated:        %d\n"
		  "checksum:         0x%x\n"
		  "prev:	     %p\n"
		  "next:	     %p\n",
		  this,
//		  (void*)getAddress(),
		  size,
		  allocated,
		  checksum,
		  prevSegment,
		  nextSegment);
   }

private:
   std::size_t size;

   Segment* prevSegment;
   Segment* nextSegment;

   bool allocated;
   uint8_t checksum;
   friend class SegmentList;
} __attribute__((aligned(16)));

#endif
