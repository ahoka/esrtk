#ifndef MEMORYMANAGER_HH
#define	MEMORYMANAGER_HH

#include <DoublyLinkedItem.hh>
#include <DoublyLinkedList.hh>

#include <cstdint>

class MemoryManager
{
public:
	MemoryManager();
	MemoryManager(const MemoryManager& orig);
	virtual ~MemoryManager();
private:

   class Segment : public DoublyLinkedItem<Segment>
   {
   public:
      Segment() :
	 address(0),
	 size(0)
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

   private:
      uintptr_t address;
      uintptr_t size;
   };
};

#endif	/* MEMORYMANAGER_H */

