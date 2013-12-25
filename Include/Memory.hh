#ifndef MEMORY_HH
#define MEMORY_HH

#include <Debug.hh>
#include <Parameters.hh>

#include <cstdint>
#include <cstddef>

extern void* __start_text;
extern void* __end_text;
extern void* __start_rodata;
extern void* __end_rodata;
extern void* __start_data;
extern void* __end_data;
extern void* __start_bss;
extern void* __end_bss;
extern void* __end_kernel;

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

enum
{
   MemoryMapMax = 32,
   PhysMapSize = 128 * 1024
};

class PhysicalPage
{
public:
   enum
   {
      // extreme value
      Invalid = ~0UL
   };

   PhysicalPage()
      : prev(this),
	next(this),
	address(Invalid)
   {
      // empty
   }

   uintptr_t getAddress()
   {
      return address & AddressMask;
   }

   // this wont preserve color
   void setAddress(uintptr_t addr)
   {
      address = addr;
   }

   void setColor(uint8_t color)
   {
      address &= AddressMask;
      address |= color;
   }

   uint8_t getColor()
   {
      return static_cast<uint8_t>(address & ColorMask);
   }

   bool isValid()
   {
      return address == Invalid;
   }

   bool isInvalid()
   {
      return !isValid();
   }

private:
   friend class PageCluster;

   PhysicalPage* prev;
   PhysicalPage* next;
   uintptr_t address;

   enum
   {
      AddressMask = ~PageMask,
      ColorMask = PageMask
   };
};

class PageCluster
{
public:
   enum
   {
      Free,
      Anonymous,
      KernelHeap,
      KernelStack
   };

   void insert(PhysicalPage* page)
   {
      KASSERT(page != 0);      

      PhysicalPage* oldNext = head.next;

      page->next = head.next;
      head.next = page;

      oldNext->prev = page;
      page->prev = &head;
   }

   void remove(PhysicalPage* page)
   {
      KASSERT(page != 0);

      PhysicalPage* oldPrev = page->prev;
      PhysicalPage* oldNext = page->next;

      oldPrev->next = oldNext;
      oldNext->prev = oldPrev;
   }

   PhysicalPage* get()
   {
      PhysicalPage* page = 0;

      if (head.next->getAddress() != PhysicalPage::Invalid)
      {
	 page = head.next;
	 remove(head.next);
      }

      return page;
   }

   PhysicalPage* find(uintptr_t address)
   {
      for (PhysicalPage* page = head.next;
	   page->getAddress() != PhysicalPage::Invalid;
	   page = page->next)
      {
	 if (page->getAddress() == address)
	 {
	    return page;
	 }
      }

      return 0;
   }

   void init()
   {
      // this is our elephant in cairo :-)
      head.setAddress(PhysicalPage::Invalid);
      head.next = &head;
      head.prev = &head;
   }

private:
   PhysicalPage head;
};

struct InterruptFrame;

class Memory
{
public:
   static void init();
   static bool handlePageFault(uintptr_t address, InterruptFrame* frame);
   static void copyMemoryMap();

   static uintptr_t getPage();
   static void putPage(uintptr_t paddr);
   static bool mapPage(uintptr_t vaddr, uintptr_t paddr);
   static uintptr_t mapPage(uintptr_t paddr);
   static bool unmapPage(uintptr_t vaddr);
   static uintptr_t mapRegion(uintptr_t paddr, std::size_t size);
   static bool unmapRegion(uintptr_t paddr, std::size_t size);
   static uintptr_t mapAnonymousRegion(std::size_t size);
   static void* readPhysicalMemory(void* destination, const void* source, std::size_t size);
   static bool createKernelStack(uintptr_t& start);

private:
   Memory();

   static uintptr_t heapEnd;
   static uintptr_t stackEnd;
   static uintptr_t mapEnd;

   static PageCluster usedPages;
   static PageCluster freePages;

   static MemorySegment memoryMap[MemoryMapMax];
   static unsigned int memoryMapCount;
};

#endif
