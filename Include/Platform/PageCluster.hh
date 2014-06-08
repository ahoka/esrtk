#ifndef PAGECLUSTER_HH
#define PAGECLUSTER_HH

#include <cassert>

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

   PageCluster()
   {
      // must be empty
   }

   void insert(PhysicalPage* page)
   {
      assert(page != 0);

      PhysicalPage* oldNext = head.next;

      assert(oldNext != 0);

      page->next = head.next;
      head.next = page;

      oldNext->prev = page;
      page->prev = &head;
   }

   void remove(PhysicalPage* page)
   {
      assert(page != 0);

      PhysicalPage* oldPrev = page->prev;
      PhysicalPage* oldNext = page->next;

      assert(oldPrev != 0);
      assert(oldNext != 0);

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
      head.init();
      head.next = &head;
      head.prev = &head;
   }

private:
   PhysicalPage head;
};

#endif
