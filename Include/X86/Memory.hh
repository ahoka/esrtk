#ifndef MEMORY_HH
#define MEMORY_HH

#include <Multiboot.hh>
#include <X86/Idt.hh>

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
   MemoryMapMax = 16,
   PhysMapSize = 128 * 1024
};

struct PhysicalPage
{
   uintptr_t address;
//   uint32_t flags;

   PhysicalPage* prev;
   PhysicalPage* next;

   enum
   {
      // extreme value
      Invalid = ~0UL
   };

   PhysicalPage() :
      address(0),
//      flags(0),
      prev(0),
      next(0)
   {
      // empty
   }
};

class PageCluster
{
public:
//   uint32_t color;

   void insert(PhysicalPage* page)
   {
//      printf("head: %p, p %p, n %p\n", &head, head.prev, head.next);

      PhysicalPage* oldNext = head.next;

      page->next = head.next;
      head.next = page;

      oldNext->prev = page;
      page->prev = &head;
   }

   void remove(PhysicalPage* page)
   {
      PhysicalPage* oldPrev = page->prev;
      PhysicalPage* oldNext = page->next;

      oldPrev->next = oldNext;
      oldNext->prev = oldPrev;
   }

   PhysicalPage* get()
   {
      PhysicalPage* page = 0;

      if (head.next->address != PhysicalPage::Invalid)
      {
	 page = head.next;
	 remove(head.next);
      }

      return page;
   }

   PhysicalPage* find(uintptr_t address)
   {
      for (PhysicalPage* page = head.next;
	   page->address != PhysicalPage::Invalid;
	   page = page->next)
      {
	 if (page->address == address)
	 {
	    return page;
	 }
      }

      return 0;
   }

   void init()
   {
      // this is our elephant in cairo :-)
      head.address = PhysicalPage::Invalid;
      head.next = &head;
      head.prev = &head;

//      printf("head: %p, p %p, n %p\n", &head, head.prev, head.next);
   }

private:
   PhysicalPage head;
};

class Memory
{
public:
   static void init();
   static bool handlePageFault(uintptr_t address, InterruptFrame* frame);
   static void copyMemoryMap();

   static uintptr_t sbrk(std::size_t size);
   static uintptr_t getPage();
   static void putPage(uintptr_t paddr);
   static bool mapPage(uintptr_t vaddr, uintptr_t paddr);
   static uintptr_t mapPage(uintptr_t paddr);
   static bool unmapPage(uintptr_t vaddr);
   static uintptr_t mapRegion(uintptr_t paddr, std::size_t size);
   static bool unmapRegion(uintptr_t paddr, std::size_t size);

private:
   Memory();

   static uintptr_t heapEnd;
   static uintptr_t stackEnd;
   static uintptr_t mapEnd;

   static PageCluster usedPages;
   static PageCluster freePages;

   static MemorySegment memoryMap[16];
   static unsigned int memoryMapCount;
};

#endif
