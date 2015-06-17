#ifndef PAGECLUSTER_HH
#define PAGECLUSTER_HH

#include <cassert>

class PageCluster
{
public:
   // must be empty
   PageCluster()
   {
   }

   void init()
   {
      // this is our elephant in cairo :-)
      headM.init();
      headM.next = &headM;
      headM.prev = &headM;
      countM = 0;
   }

   void insert(PhysicalPage* page)
   {
      KASSERT(page != 0);

      PhysicalPage* oldNext = headM.next;

      KASSERT(oldNext != 0);

      page->next = headM.next;
      headM.next = page;

      oldNext->prev = page;
      page->prev = &headM;

      ++countM;
   }

   void remove(PhysicalPage* page)
   {
      KASSERT(page != 0);

      PhysicalPage* oldPrev = page->prev;
      PhysicalPage* oldNext = page->next;

      KASSERT(oldPrev != 0);
      KASSERT(oldNext != 0);

      oldPrev->next = oldNext;
      oldNext->prev = oldPrev;

      --countM;
   }

   PhysicalPage* get()
   {
      PhysicalPage* page = 0;

      if (headM.next->getAddress() != PhysicalPage::Invalid)
      {
	 page = headM.next;
	 remove(headM.next);
      }

      return page;
   }

   PhysicalPage* find(uintptr_t address)
   {
      for (PhysicalPage* page = headM.next;
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

   unsigned long count() const
   {
      return countM;
   }

private:
   PhysicalPage headM;
   unsigned long countM;
};

#endif
