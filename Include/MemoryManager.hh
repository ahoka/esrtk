#ifndef MEMORYMANAGER_HH
#define	MEMORYMANAGER_HH

#include <DoublyLinkedItem.hh>
#include <DoublyLinkedList.hh>

#include <Templates.hh>
#include <Parameters.hh>

#include <cstdint>
#include <cstddef>

#include <Debug.hh>

class MemoryManager
{
public:
   MemoryManager();
   ~MemoryManager();

   static void init();

   void* allocate(std::size_t size);
   void deallocate(void *data);
   void printStatistics();

   static MemoryManager& get();

private:
   MemoryManager(const MemoryManager& orig);

   void* operator new(std::size_t, void *);

   static void* allocateBackend(std::size_t size);

   class Segment : public DoublyLinkedItem<Segment>
   {
   public:
      Segment() :
	 address(0),
	 size(0),
	 allocated(false)
      {
         Debug::verbose("Segment(%p)\n", this);
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
                     address,
                     size,
                     allocated,
                     checksum);
      }

   private:
      uintptr_t address;
      uintptr_t size;
      bool allocated;
      uint8_t checksum;

      // Segment* prevSegment;
      // Segment* nextSegment;
   } __attribute__((aligned(16)));

   static Segment* headerOf(void* segment);

   DoublyLinkedList<Segment> freeList;

   static MemoryManager* self;
};

#endif	/* MEMORYMANAGER_H */

