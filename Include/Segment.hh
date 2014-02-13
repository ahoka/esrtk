#ifndef SEGMENT_HH
#define SEGMENT_HH

class SegmentList;

class Segment
{
public:
   Segment() :
      address(0),
      size(0),
      allocated(false),
      prevSegment(this),
      nextSegment(this)
   {
      Debug::verbose("Segment(%p)\n", this);
      // address = 0;
      // size = 0;
      // allocated = false;
      updateChecksum();
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

   void setAddress(uintptr_t newAddress)
   {
      KASSERT(verifyChecksum());
      KASSERT(newAddress == (uintptr_t(this) + sizeof(*this)));

      address = newAddress;

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

      return address;
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

      uint8_t* p = (uint8_t*)this;
      uint8_t sum = 0;
      for (std::size_t i = 0; i < sizeof(*this); i++, p++)
      {
	 sum ^= *p;
      }

      Debug::verbose("Checksum updated: %u\n", sum);
      dump();

      checksum = sum;

      verifyChecksum();
   }

   bool verifyChecksum()
   {
      uint8_t* p = (uint8_t*)this;
      uint8_t sum = 0;
      for (std::size_t i = 0; i < sizeof(*this); i++, p++)
      {
	 sum ^= *p;
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

   void dump()
   {
      Debug::info("segment:          %p\n"
		  "address:          %p\n"
		  "size:             %zu\n"
		  "allocated:        %d\n"
		  "checksum:         0x%x\n",
		  this,
		  (void*)address,
		  (std::size_t)size,
		  allocated,
		  checksum);
   }

private:
   uintptr_t address;
   uintptr_t size;
   bool allocated;
   uint8_t checksum;

   Segment* prevSegment;
   Segment* nextSegment;

   friend class SegmentList;
} __attribute__((aligned(16)));

#endif
