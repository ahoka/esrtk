#ifndef MEMORYMANAGER_HH
#define	MEMORYMANAGER_HH

#include <DoublyLinkedItem.hh>
#include <DoublyLinkedList.hh>

#include <Templates.hh>
#include <Parameters.hh>

#include <cstdint>
#include <cstddef>

class MemoryManager
{
public:
   MemoryManager();
   ~MemoryManager();

   static void init();

   void* allocate(std::size_t size);
   void deallocate(void *data);

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
	 // empty
      }

      void setSize(uintptr_t newSize)
      {
	 size = newSize;
      }

      void setAddress(uintptr_t newAddress)
      {
	 address = newAddress;
      }

      uintptr_t getSize()
      {
	 return size;
      }

      uintptr_t getAddress()
      {
	 return address;
      }

      bool isAllocated()
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

   private:
      uintptr_t address;
      uintptr_t size;
      bool allocated;

      // Segment* prevSegment;
      // Segment* nextSegment;
   };

   static Segment* headerOf(void* segment);

   DoublyLinkedList<Segment> freeList;

   static MemoryManager* self;
};

#endif	/* MEMORYMANAGER_H */

